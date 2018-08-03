#include "catalog.hpp"

/// @abi table
struct entry {
  uint64_t      id;
  account_name  owner;
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
  account_name get_owner()const { return owner; }

  bool operator==( const entry& b ) const {
    return (this->owner == b.owner && this->tokenname == b.tokenname);
  }

  bool operator!=( const entry& b ) const {
    return (this->owner != b.owner || this->tokenname != b.tokenname);
  }
  
};

class tokencatalog : public catalog<entry> {
public:
  tokencatalog( account_name self ):
    catalog<entry>(self)
  {}

  const int MAX_VALUE_LEN = 1024;
  
  void transferAction (uint64_t code)
  {
    auto data = unpack_action_data<currency::transfer>();
    eosio_assert(data.quantity.is_valid(), "Invalid quantity");
    extended_asset payment(data.quantity, code);

    if(data.from == _self) {
      refund(data.to, payment);
      return;
    }
    
    verify_symbol_str(data.memo);
    
    entry e;
    e.owner = data.from;
    e.tokenname = data.memo;
    pay_add_entry(payment, e);
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
    switch(key) {
    case N(title):   ent.title = value;
      break;
    case N(org):     ent.org = value;
      break;
    case N(urlwebsite): ent.urlwebsite = value;
      break;
    case N(urllogo): ent.urllogo = value;
      break;
    case N(urlsrccode): ent.urlsrccode = value;
      break;
    case N(email):   ent.email = value;
      break;
    case N(descr):   ent.descr = value;
      break;
    default:
      eosio_assert(0, "Unknown key name");
    }
  }
  
                 
  
  /// @abi action
  void setvalue (account_name owner, string tokenname, name key, string value)
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
    
  /// @abi action
  void setvalues (account_name owner, string tokenname, vector<keyval> values)
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
  
  
  /// @abi action
  void settags (account_name owner, string tokenname, vector<name>& tags)
  {
    require_owner_or_delegate_auth( owner );
    entry e;
    e.owner = owner;
    e.tokenname = tokenname;
    set_entry_tags( e, tags );
  }

  
  /// @abi action
  void setflag (account_name owner, string tokenname, name flag)
  {
    require_owner_or_delegate_auth( owner );
    entry e;
    e.owner = owner;
    e.tokenname = tokenname;
    set_entry_flag( e, flag );
  }

  /// @abi action
  void modtokenname (account_name owner, string oldtokenname, string newtokenname)
  {
    require_owner_or_delegate_auth( owner );    
    verify_symbol_str(newtokenname);

    entry e;
    e.owner = owner;
    e.tokenname = newtokenname;
    eosio_assert( !entry_exists(e), "Such toke nname already exists");

    e.tokenname = oldtokenname;
    modify_entry( e, [&]( auto& ent ) {
        ent.tokenname = newtokenname;
      });
  }
};


extern "C" void apply(uint64_t receiver, uint64_t code, uint64_t action) {
  if (code == N(eosio.token) && action == N(transfer)) {
    tokencatalog thiscontract(receiver);
    thiscontract.transferAction(code);
  }
  else if( code == receiver ) {
    tokencatalog thiscontract(receiver);
    switch( action ) {
      EOSIO_API( tokencatalog,
                 (setprice)(setvalue)(setvalues)(settags)(setflag)(modtokenname)(claimrefund)(delegate) );
    }                                       
  }
}

