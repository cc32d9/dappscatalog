#include "catalog.hpp"

/// @abi table
struct entry {
  uint64_t      id;
  account_name  code;
  string        symbol;
  string        title;
  string        url;
  string        descr;
  uint64_t      flags = 0;

  auto primary_key()const { return id; }
  account_name get_code()const { return code; }

  bool operator==( const entry& b ) const {
    return (this->code == b.code && this->symbol == b.symbol);
  }

  bool operator!=( const entry& b ) const {
    return (this->code != b.code || this->symbol != b.symbol);
  }
  
};

class tokencatalog : public catalog<entry> {
public:
  tokencatalog( account_name self ):
    catalog<entry>(self)
  {}

  void transferAction (uint64_t code)
  {
    auto data = unpack_action_data<currency::transfer>();
    if(data.from == _self || data.to != _self) {
      return;
    }

    eosio_assert(data.quantity.is_valid(), "Invalid quantity");
    extended_asset payment(data.quantity, code);
    
    eosio_assert(data.memo.length() > 0, "Empty memo. A symbol name expected");
    eosio_assert(data.memo.length() <= 7, "memo is too long. A symbol name expected");
    for( int i = 0; i < data.memo.length(); i++ ) {
      char c = data.memo[i];
      eosio_assert('A' <= c && c <= 'Z', "Invalid character. A valid symbol name expected");
    }
    
    entry ent;
    ent.code = data.from;
    ent.symbol = data.memo;
    pay_add_entry(payment, ent);
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
      EOSIO_API( tokencatalog, (setprice) );
    }                                       
  }
}

