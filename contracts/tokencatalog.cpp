#include "catalog.hpp"


class tokencatalog : public catalog {
public:
  tokencatalog( account_name self ):
    catalog(self)
  {}

  /// @abi table
  struct entry {
    uint64_t      id;
    name          owner;
    symbol_type   symbol;
    string        title;
    string        url;
    string        descr;
    auto primary_key()const { return id; }
    account_name get_owner()const { return owner; }
  };

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

