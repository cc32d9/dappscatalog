#include <eosiolib/eosio.hpp>
#include <eosiolib/action.hpp>
#include <eosiolib/currency.hpp>
#include <eosiolib/multi_index.hpp>


using namespace eosio;

template<typename ENTRY>
class catalog : public eosio::contract {
public:
  catalog<ENTRY>( account_name self ):
    contract(self), _tagcloud(self, self), _prices(self, self),
    _entries(self, self)
  {}

  typedef eosio::multi_index<N(entries), ENTRY,
    indexed_by<N(code), const_mem_fun<ENTRY, uint64_t, &ENTRY::get_code>>> entries;

  void pay_addentry( extended_asset payment, ENTRY& ent )
  {
    uint64_t exactprice = 0;
    
    auto codeidx = _entries.template get_index<N(code)>();
    auto itr = codeidx.lower_bound(ent.code);
    if( itr != codeidx.end() ) {
      // one entry exists. Assert that we don't have equal entries
      while(itr != codeidx.end() && itr->code == ent.code) {
        eosio_assert(*itr != ent, "The same entry already exists");
        itr++;
      }

      exactprice = get_price_obj(payment.contract, payment.symbol).psubentry;
    }
    else {
      exactprice = get_price_obj(payment.contract, payment.symbol).pnewentry;
    }

    // check if the payment amount is right
    eosio_assert(payment.amount >= exactprice, "Payment amount is too low");
    eosio_assert(payment.amount == exactprice, "Payment amount is too high");
    
    _entries.emplace(_self, [&]( auto& constrent ) {
        constrent = ent;
      });
  }

  void set_price(const account_name code, const symbol_type symbol,
                 uint64_t pnewentry, uint64_t psubentry)
  {
    auto codeidx = _prices.template get_index<N(code)>();
    auto itr = codeidx.lower_bound(code);
    while(itr != codeidx.end() && itr->code == code) {
      if( itr->symbol == symbol ) {
        break;
      }
      itr++;
    }

    if( itr == codeidx.end() ) {
      _prices.emplace(_self, [&]( auto& p ) {
          p.id = _prices.available_primary_key();
          p.code = code;
          p.symbol = symbol;
          p.pnewentry = pnewentry;
          p.psubentry = psubentry;
        });
    } else {
      _prices.modify( *itr, _self, [&]( auto& p ) {
          p.pnewentry = pnewentry;
          p.psubentry = psubentry;
        });
    }
  }

  /// @abi table
  struct tag {
    uint64_t       id;
    uint64_t       entry_id;
    name           tag;
    auto primary_key()const { return id; }
    uint64_t get_tag()const { return tag; }
    uint64_t get_entry_id()const { return entry_id; }
  };

  typedef eosio::multi_index<N(tags), tag,
    indexed_by<N(tag), const_mem_fun<tag, uint64_t, &tag::get_tag>>,
    indexed_by<N(entryid), const_mem_fun<tag, uint64_t, &tag::get_entry_id>>> tagcloud;
    
  void set_tags(uint64_t entry_id, vector<name> tags)
  {
    // delete all tags for an entry
    auto tagidx = _tagcloud.template get_index<N(entryid)>();
    auto itr = tagidx.lower_bound(entry_id);
    while(itr != tagidx.end() && itr->entry_id == entry_id ) {
      tagidx.erase(itr);
      itr++;
    }
    
    for( name tag : tags ) {
      _tagcloud.emplace(_self, [&]( auto& p ) {
          p.id = _tagcloud.available_primary_key();
          p.entry_id = entry_id;
          p.tag = tag;
        });
    }
  }
  
private:

  /// @abi table
  struct price {
    uint64_t       id;
    account_name   code;
    symbol_type    symbol;
    uint64_t       pnewentry;
    uint64_t       psubentry;
    auto primary_key()const { return id; }
    uint64_t get_code()const { return code; }
  };

  typedef eosio::multi_index<N(prices), price,
    indexed_by<N(code), const_mem_fun<price, uint64_t, &price::get_code>>> prices;

  const price& get_price_obj(const account_name code, const symbol_type symbol)
  {
    auto codeidx = _prices.template get_index<N(code)>();
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
    
  prices _prices;
  tagcloud _tagcloud;
  entries _entries;
};


  
  
