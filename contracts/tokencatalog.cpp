#include "catalog.hpp"

/// @abi table
struct entry {
  uint64_t      id;
  account_name  code;
  symbol_type   symbol;
  string        title;
  string        url;
  string        descr;
  uint64_t      flags;

  auto primary_key()const { return id; }
  account_name get_code()const { return code; }

  bool operator==( const entry& b ) {
    return (this->code == b.code && this->symbol == b.symbol);
  }

};

class tokencatalog : public catalog<entry> {
public:
  tokencatalog( account_name self ):
    catalog<entry>(self)
  {}


  void transferAction (uint64_t self, uint64_t code)
  {
    auto data = unpack_action_data<currency::transfer>();
    if(data.from == self || data.to != self) {
      return;
    }
  
    eosio_assert(data.quantity.is_valid(), "Invalid quantity");
    auto symbol = data.quantity.symbol;

  }
};


extern "C" void apply(uint64_t receiver, uint64_t code, uint64_t action) {
  auto self = receiver;
  tokencatalog thiscontract(self);
  
  if (code == N(eosio.token) && action == N(transfer)) {
    thiscontract.transferAction(receiver, code);
  }  
}

