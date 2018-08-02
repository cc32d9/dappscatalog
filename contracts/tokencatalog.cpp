#include "catalog.hpp"

/// @abi table
struct entry {
  uint64_t      id;
  account_name  owner;
  string        symbol;
  string        title;
  string        org;
  string        url;
  string        email;
  string        descr;
  uint64_t      flags = 0;

  auto primary_key()const { return id; }
  account_name get_owner()const { return owner; }

  bool operator==( const entry& b ) const {
    return (this->owner == b.owner && this->symbol == b.symbol);
  }

  bool operator!=( const entry& b ) const {
    return (this->owner != b.owner || this->symbol != b.symbol);
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
    
    eosio_assert(data.memo.length() > 0, "Empty memo. A symbol name expected");
    verify_symbol_str(data.memo);
    
    entry e;
    e.owner = data.from;
    e.symbol = data.memo;
    pay_add_entry(payment, e);
  }

  void verify_symbol_str(const string& str)
  {
    eosio_assert(str.length() > 0, "Empty string. A symbol name expected");
    eosio_assert(str.length() <= 7, "Suymbol name is longer than 7 characters");
    for( int i = 0; i < str.length(); i++ ) {
      char c = str[i];
      eosio_assert('A' <= c && c <= 'Z', "Invalid character in symbol name. Expected [A-Z]{1,7}");
    }
  }

      
   /// @abi action
  void setvalue (account_name owner, string symbol, name key, string value)
  {
    require_owner_or_delegate_auth( owner );
    eosio_assert( value.length() <= MAX_VALUE_LEN, "Value is too long" );
    entry e;
    e.owner = owner;
    e.symbol = symbol;
    modify_entry( e, [&]( auto& ent ) {
        switch(key) {
        case N(title):   ent.title = value;
          break;
        case N(org):     ent.org = value;
          break;
        case N(url):     ent.url = value;
          break;
        case N(email):   ent.email = value;
          break;
        case N(descr):   ent.descr = value;
          break;
        default: eosio_assert(0, "Unknown key name");
        }});
  }

  struct keyval {
    name     key;
    string   value;
  };
    
  /// @abi action
  void setvalues (account_name owner, string symbol, vector<keyval> values)
  {
    require_owner_or_delegate_auth( owner );
    for(auto kv: values) {
      eosio_assert( kv.value.length() <= MAX_VALUE_LEN, "Value is too long" );
    }

    entry e;
    e.owner = owner;
    e.symbol = symbol;
    modify_entry( e, [&]( auto& ent ) {
        for(auto kv: values) {
          switch(kv.key) {
          case N(title):    ent.title = kv.value;
            break;
          case N(org):      ent.org = kv.value;
            break;
          case N(url):      ent.url = kv.value;
            break;
          case N(email):    ent.email = kv.value;
            break;
          case N(descr):    ent.descr = kv.value;
            break;
          default: eosio_assert(0, "Unknown key name");
          }
        }
      });
  }
  
  
  /// @abi action
  void settags (account_name owner, string symbol, vector<name>& tags)
  {
    require_owner_or_delegate_auth( owner );
    entry e;
    e.owner = owner;
    e.symbol = symbol;
    set_entry_tags( e, tags );
  }

  
  /// @abi action
  void setflag (account_name owner, string symbol, name flag)
  {
    require_owner_or_delegate_auth( owner );
    entry e;
    e.owner = owner;
    e.symbol = symbol;
    set_entry_flag( e, flag );
  }

  /// @abi action
  void modsymbol (account_name owner, string oldsymbol, string newsymbol)
  {
    require_owner_or_delegate_auth( owner );    
    verify_symbol_str(newsymbol);

    entry e;
    e.owner = owner;
    e.symbol = newsymbol;
    eosio_assert( !entry_exists(e), "Such symbol already exists");

    e.symbol = oldsymbol;
    modify_entry( e, [&]( auto& ent ) {
        ent.symbol = newsymbol;
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
                 (setprice)(setvalue)(setvalues)(settags)(setflag)(modsymbol)(claimrefund)(delegate) );
    }                                       
  }
}

