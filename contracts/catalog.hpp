#include <eosiolib/eosio.hpp>
#include <eosiolib/action.hpp>
#include <eosiolib/currency.hpp>
#include <eosiolib/multi_index.hpp>


using namespace eosio;

template<typename ENTRY>
class catalog : public eosio::contract {
public:
  catalog( account_name self ):
    contract(self), _tagcloud(self, self), _prices(self, self),
    _entries(self, self), _payments(self, self)
  {}

  typedef eosio::multi_index<N(entry), ENTRY,
    indexed_by<N(owner), const_mem_fun<ENTRY, uint64_t, &ENTRY::get_owner>>> entries;

  void pay_add_entry( const extended_asset& payment, const ENTRY& ent )
  {
    uint64_t exactprice = 0;
    
    auto owneridx = _entries.template get_index<N(owner)>();
    auto itr = owneridx.lower_bound(ent.owner);
    if( itr != owneridx.end() ) {
      // one entry exists. Assert that we don't have equal entries
      while(itr != owneridx.end() && itr->owner == ent.owner) {
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
    
    _entries.emplace(_self, [&]( auto& ce ) {
        ce = ent;
        ce.id = _entries.available_primary_key();
      });

    // remember the payment for possible refunding
    register_payment(ent.owner, payment);  
  }

  
  template<typename Lambda>
  void modify_entry(const ENTRY& e, Lambda&& updater )
  {
    auto itr = get_entry(e);
    _entries.modify( *itr, _self, std::forward<Lambda&&>(updater) );
  }

  
  void set_entry_flag(const ENTRY& e, name flag)
  {
    auto itr = get_entry(e);
    _entries.modify( *itr, _self, [&]( auto& ent ) {
        switch(flag) {
        case N(complete): ent.flags |= 1UL;
          break;
        case N(incomplete): ent.flags &= ~(1UL);
          break;
        case N(show): ent.flags |= 2UL;
          break;
        case N(hide): ent.flags &= ~(2UL);
          break;
        default: eosio_assert(0, "Unknown flag name");
        }});
  }    
  
  /// @abi action
  void setprice(const account_name code,
                const asset& price_newentry,
                const asset& price_subentry)
  {
    require_auth( _self );
    eosio_assert(price_newentry.symbol == price_subentry.symbol,
                 "Newentry and subentry prices are in different currency");
    auto codeidx = _prices.template get_index<N(code)>();
    auto itr = codeidx.lower_bound(code);
    while(itr != codeidx.end() && itr->code == code &&
          itr->symbol != price_newentry.symbol ) {
      itr++;
    }

    if( itr != codeidx.end() && itr->code == code &&
        itr->symbol == price_newentry.symbol ) {
      _prices.modify( *itr, _self, [&]( auto& p ) {
          p.pnewentry = price_newentry.amount;
          p.psubentry = price_subentry.amount;
        });
    } else {
      _prices.emplace(_self, [&]( auto& p ) {
          p.id = _prices.available_primary_key();
          p.code = code;
          p.symbol = price_newentry.symbol;
          p.pnewentry = price_newentry.amount;
          p.psubentry = price_subentry.amount;
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

  typedef eosio::multi_index<N(tag), tag,
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

  
  /// @abi action
  void refund(account_name owner)
  {
    require_auth(owner);

    // erase tagcloud and entries
    auto entidx = _entries.template get_index<N(owner)>();
    auto entitr = entidx.lower_bound(owner);
    while(entitr != entidx.end() && entitr->owner == owner) {
      uint64_t entry_id = entitr->id;
      
      auto tagidx = _tagcloud.template get_index<N(entryid)>();
      auto tagitr = tagidx.lower_bound(entry_id);
      while(tagitr != tagidx.end() && tagitr->entry_id == entry_id ) {
        tagidx.erase(tagitr);
      }

      entidx.erase(entitr);
    }

    // payback and erase payments
    auto payidx = _payments.template get_index<N(owner)>();
    auto payitr = payidx.lower_bound(owner);
    while(payitr != payidx.end() && payitr->owner == owner ) {
      action {
        permission_level{_self, N(active)},
          payitr->code,
            N(transfer),
            currency::transfer{
            .from=_self,
              .to=owner,
              .quantity=asset{payitr->amount, payitr->symbol},
              .memo="refund"}
      }.send(); 
      payidx.erase(payitr);
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

  typedef eosio::multi_index<N(price), price,
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

  auto get_entry(const ENTRY& ent)
  {
    auto owneridx = _entries.template get_index<N(owner)>();
    auto itr = owneridx.lower_bound(ent.owner);
    eosio_assert( itr != owneridx.end(), "Cannot find entry for this owner");
                  
    while(itr != owneridx.end() && itr->owner == ent.owner && *itr != ent ) {
      itr++;
    }

    eosio_assert(*itr == ent, "Cannot find such item in entry table");
    return itr;
  }


  /// @abi table
  struct payment {
    uint64_t       id;
    account_name   owner;
    account_name   code;
    symbol_type    symbol;
    int64_t        amount;
    auto primary_key()const { return id; }
    uint64_t get_owner()const { return owner; }
  };

  typedef eosio::multi_index<N(payment), payment,
    indexed_by<N(owner), const_mem_fun<payment, uint64_t, &payment::get_owner>>> payments;

  void register_payment(account_name owner, const extended_asset& payment)
  {
    auto owneridx = _payments.template get_index<N(owner)>();
    auto itr = owneridx.lower_bound(owner);
    while(itr != owneridx.end() && itr->owner == owner &&
          itr->code != payment.contract && itr->symbol != payment.symbol ) {
      itr++;
    }

    if( itr != owneridx.end() && itr->owner == owner &&
        itr->code == payment.contract && itr->symbol == payment.symbol ) {
      _payments.modify( *itr, _self, [&]( auto& p ) {
          p.amount += payment.amount;
        });
    } else {
      _payments.emplace(_self, [&]( auto& p ) {
          p.id = _payments.available_primary_key();
          p.owner = owner;
          p.code = payment.contract;
          p.symbol = payment.symbol;
          p.amount = payment.amount;
        });
    }
  }
  
  prices _prices;
  tagcloud _tagcloud;
  entries _entries;
  payments _payments;
};


  
  
