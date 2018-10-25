#include "catalog.hpp"


struct [[eosio::table("entries")]] entry {
  uint64_t      id;
  name          owner;
  string        tokenname = "";
  string        title = "";
  string        org = "";
  string        urlwebsite = "";
  string        urllogo = "";
  string        urlsrccode = "";
  string        email = "";
  string        descr = "";
  uint64_t      flags = 0;

  auto primary_key()const { return id; }
  uint64_t get_owner()const { return owner.value; }

  bool operator==( const entry& b ) const {
    return (this->owner == b.owner && this->tokenname == b.tokenname);
  }

  bool operator!=( const entry& b ) const {
    return (this->owner != b.owner || this->tokenname != b.tokenname);
  }
  
}; 



class [[eosio::contract]] dappscatalog : public catalog<entry> {
public:
  dappscatalog( name self, name code, datastream<const char*> ds ):
    catalog<entry>(self, code, ds)
  {}

  const int MAX_VALUE_LEN = 1024;
  
  void transfer_action (name from, name to, asset quantity, string memo)
  {
    eosio_assert(quantity.is_valid(), "Invalid quantity");
    extended_asset payment(quantity, get_code());
    
    if(from == _self) {
      register_refund(to, payment);
      return;
    }

    if(to == _self) {
      verify_symbol_str(memo);
      entry e;
      e.owner = from;
      e.tokenname = memo;
      pay_add_entry(payment, e);
    }
  }
  

  void verify_symbol_str(const string& str)
  {
    eosio_assert(str.length() <= 7, "Symbol name is longer than 7 characters");
    for( int i = 0; i < str.length(); i++ ) {
      char c = str[i];
      eosio_assert('A' <= c && c <= 'Z', "Invalid character in symbol name. Expected [A-Z]{1,7}");
    }
  }

  
  void set_val(entry &ent, name key, string value)
  {
    switch(key.value) {
    case name("title").value:   ent.title = value;
      break;
    case name("org").value:     ent.org = value;
      break;
    case name("urlwebsite").value: ent.urlwebsite = value;
      break;
    case name("urllogo").value: ent.urllogo = value;
      break;
    case name("urlsrccode").value: ent.urlsrccode = value;
      break;
    case name("email").value:   ent.email = value;
      break;
    case name("descr").value:   ent.descr = value;
      break;
    default:
      eosio_assert(0, "Unknown key name");
    }
  }
  
                 
  
  [[eosio::action]]
  void setvalue (name owner, string tokenname, name key, string value)
  {
    require_owner_or_delegate_auth( owner );
    eosio_assert( value.length() <= MAX_VALUE_LEN, "Value is too long" );
    entry e;
    e.owner = owner;
    e.tokenname = tokenname;
    modify_entry( e, [&]( auto& ent ) { set_val(ent, key, value); });
  }


  struct keyval {
    name     key;
    string   value;
  };

  
  [[eosio::action]]
  void setvalues (name owner, string tokenname, std::vector<keyval> values)
  {
    require_owner_or_delegate_auth( owner );
    for(auto kv: values) {
      eosio_assert( kv.value.length() <= MAX_VALUE_LEN, "Value is too long" );
    }

    entry e;
    e.owner = owner;
    e.tokenname = tokenname;
    modify_entry( e, [&]( auto& ent ) {
        for(auto kv: values) {
          set_val(ent, kv.key, kv.value);
        }
      });
  }
  
  
  [[eosio::action]]
  void settags (name owner, string tokenname, std::vector<name>& tags)
  {
    require_owner_or_delegate_auth( owner );
    entry e;
    e.owner = owner;
    e.tokenname = tokenname;
    set_entry_tags( e, tags );
  }


  [[eosio::action]]
  void setattr (name owner, string tokenname, name key, const string val)
  {
    require_owner_or_delegate_auth( owner );
    entry e;
    e.owner = owner;
    e.tokenname = tokenname;
    set_attr( e, key, val );
  }

  [[eosio::action]]
  void delattr (name owner, string tokenname, name key)
  {
    require_owner_or_delegate_auth( owner );
    entry e;
    e.owner = owner;
    e.tokenname = tokenname;
    del_attr( e, key );
  }
  
  [[eosio::action]]
  void setflag (name owner, string tokenname, name flag)
  {
    require_owner_or_delegate_auth( owner );
    entry e;
    e.owner = owner;
    e.tokenname = tokenname;
    set_entry_flag( e, flag );
  }

  [[eosio::action]]
  void modtokenname (name owner, string oldtokenname, string newtokenname)
  {
    require_owner_or_delegate_auth( owner );    
    verify_symbol_str(newtokenname);

    entry e;
    e.owner = owner;
    e.tokenname = newtokenname;
    eosio_assert( !entry_exists(e), "Such token name already exists");

    e.tokenname = oldtokenname;
    modify_entry( e, [&]( auto& ent ) {
        ent.tokenname = newtokenname;
      });
  }

  [[eosio::action]]
  void blacklist(name owner)
  {
    require_auth( _self );
    entry e;
    e.owner = owner;
    e.title = "Blacklisted";
    set_blacklist_entry(e);
  }
};


extern "C" void apply(uint64_t receiver, uint64_t code, uint64_t action) {
  if( action == name("transfer").value ) {
    execute_action<dappscatalog>( eosio::name(receiver), eosio::name(code),
                                  &dappscatalog::transfer_action );
  }
  else if( code == receiver ) {
    switch( action ) {
      EOSIO_DISPATCH_HELPER( dappscatalog,
                             (setprice)(setvalue)(setvalues)(settags)(setattr)(delattr)
                             (setflag)(modtokenname)(claimrefund)(delegate)(startpromo)
                             (addvoucher)(remvoucher)(blacklist));
    }                                       
  }
}




