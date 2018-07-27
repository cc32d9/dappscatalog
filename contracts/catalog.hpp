#include <eosiolib/eosio.hpp>
#include <eosiolib/action.hpp>
#include <eosiolib/currency.hpp>
#include <eosiolib/multi_index.hpp>
#include <string>


using std::string;
using namespace eosio;


class catalog : public eosio::contract {
public:
  catalog( account_name self ):
    contract(self), _tagcloud(self, self), _prices(self, self)
  {}
  
  /// @abi table
  struct tag {
    uint64_t       id;
    uint64_t       entry_id;
    name           tag;
    auto primary_key()const { return id; }
    uint64_t get_tag()const { return tag; }
  };
  
  /// @abi table
  struct price {
    uint64_t       id;
    account_name  code;
    symbol_type    symbol;
    uint64_t       newentry;
    uint64_t       subentry;
    auto primary_key()const { return id; }
    uint64_t get_code()const { return code; }
  };

  typedef eosio::multi_index<N(tags), tag,
    indexed_by<N(tag), const_mem_fun<tag, uint64_t, &tag::get_tag>>> tagcloud;

  typedef eosio::multi_index<N(prices), price,
    indexed_by<N(code), const_mem_fun<price, uint64_t, &price::get_code>>> prices;
  
  
  const price& get_price(const account_name code, const symbol_type symbol)
  {
    auto codeidx = _prices.get_index<N(code)>();
    auto itr = codeidx.lower_bound(code);
    eosio_assert(itr != codeidx.end(), "Unknown currency issuer");
    while(itr->code == code) {
      if( itr->symbol == symbol ) {
        return *itr;
      }
      itr++;
    }
    eosio_assert(0, "Unknown currency name");
    return *itr;
  }
  
private:
  tagcloud _tagcloud;
  prices _prices;
};


  
  
