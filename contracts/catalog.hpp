#include <eosiolib/eosio.hpp>
#include <eosiolib/action.hpp>
#include <eosiolib/currency.hpp>
#include <eosiolib/multi_index.hpp>


using namespace eosio;


class catalog : public eosio::contract {
public:
  catalog( account_name self ):
    contract(self), _tagcloud(self, self), _prices(self, self)
  {}
  
  
  /// @abi table
  struct price {
    uint64_t       id;
    account_name  code;
    symbol_type    symbol;
    uint64_t       pnewentry;
    uint64_t       psubentry;
    auto primary_key()const { return id; }
    uint64_t get_code()const { return code; }
  };

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

  void set_price(const account_name code, const symbol_type symbol,
                 uint64_t pnewentry, uint64_t psubentry)
  {
    auto codeidx = _prices.get_index<N(code)>();
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
    
  vector<uint64_t> entries_by_tag( const name searchtag )
  {
    vector<uint64_t> ret;
    auto tagidx = _tagcloud.get_index<N(tag)>();
    auto itr = tagidx.lower_bound(searchtag);
    while(itr != tagidx.end() && itr->tag == searchtag ) {
      ret.emplace_back(itr->entry_id);
      itr++;
    }
    return ret;
  }

  vector<name> tags_by_entry( const uint64_t entry_id )
  {
    vector<name> ret;
    auto tagidx = _tagcloud.get_index<N(entryid)>();
    auto itr = tagidx.lower_bound(entry_id);
    while(itr != tagidx.end() && itr->tag == entry_id ) {
      ret.emplace_back(itr->tag);
      itr++;
    }
    return ret;
  }

  
  void set_tags(uint64_t entry_id, vector<name> tags)
  {
    // delete all tags for an entry
    auto tagidx = _tagcloud.get_index<N(entryid)>();
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
  prices _prices;
  tagcloud _tagcloud;
};


  
  
