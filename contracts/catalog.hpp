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
    _entries(self, self), _payments(self, self), _reps(self, self)
  {}

  const float REFUND_RATE = 0.9;
  const int MAX_TAGS = 10;
  bool autorefund = true;
  
  typedef eosio::multi_index<N(entry), ENTRY,
    indexed_by<N(owner), const_mem_fun<ENTRY, uint64_t, &ENTRY::get_owner>>> entries;

  bool entry_exists( const ENTRY& ent )
  {
    auto entidx = _entries.template get_index<N(owner)>();
    auto entitr = entidx.lower_bound(ent.owner);
    while(entitr != entidx.end() && entitr->owner == ent.owner) {
      if( *entitr == ent ) {
        return true;
      }
      entitr++;
    }
    return false;
  }
    
  void pay_add_entry( const extended_asset& payment, const ENTRY& ent )
  {
    eosio_assert( !has_open_refund(ent.owner),
                  "An unpaid refund is pending for this owner" );

    asset exactprice;
    auto priceobj = get_price_obj(payment.contract, payment.symbol);
    
    auto entidx = _entries.template get_index<N(owner)>();
    auto entitr = entidx.lower_bound(ent.owner);
    bool found = false;
    while(entitr != entidx.end() && entitr->owner == ent.owner) {
      found = true;
      eosio_assert(*entitr != ent, "The same entry already exists");
      entitr++;
    }

    if( found ) {
      exactprice = priceobj.psubentry;
    } else {
      exactprice = priceobj.pnewentry;
    }

    // check if the payment amount is right

    if( payment != exactprice ) {
      int64_t p = (int64_t)exactprice.symbol.precision();
      
      int64_t p10 = 1;
      while( p > 0  ) {
        p10 *= 10; --p;
      }
      p = (int64_t)exactprice.symbol.precision();

      char fraction[p+1];
      fraction[p] = '\0';
      auto change = exactprice.amount % p10;
      
      for( int64_t i = p -1; i >= 0; --i ) {
        fraction[i] = (change % 10) + '0';
        change /= 10;
      }

      char symname[9];
      auto sym = exactprice.symbol.name();
      for( int i = 0; i < 7; ++i ) {
        char c = (char)(sym & 0xff);
        symname[i] = c;
        if( c == 0 ) break;
        sym >>= 8;
      }

      char str[256];
      sprintf(str, "Incorrect payment amount. Expected %lld.%s %s",
              exactprice.amount / p10, fraction, symname);
      eosio_assert(0, str);
    }
    
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
    auto entitr = get_entry(e);
    _entries.modify( *entitr, _self, std::forward<Lambda&&>(updater) );
  }

  
  void set_entry_flag(const ENTRY& e, name flag)
  {
    auto entitr = get_entry(e);
    _entries.modify( *entitr, _self, [&]( auto& ent ) {
        switch(flag) {
        case N(ready): ent.flags |= 3UL;
          break;
        case N(complete): ent.flags |= 1UL;
          break;
        case N(incomplete): ent.flags &= ~(1UL);
          break;
        case N(show): ent.flags |= 2UL;
          break;
        case N(hide): ent.flags &= ~(2UL);
          break;
        default: eosio_assert(0, "Wrong flag name. Expected ready|complete|incomplete|show|hide");
        }});
  }    
  
  /// @abi action
  void setprice(account_name contract,
                const asset& price_newentry,
                const asset& price_subentry)
  {
    require_auth( _self );
    eosio_assert(price_newentry.symbol == price_subentry.symbol,
                 "Newentry and subentry prices are in different currency");
    auto codeidx = _prices.template get_index<N(contract)>();
    auto itr = codeidx.lower_bound(contract);
    while(itr != codeidx.end() &&
          itr->contract == contract &&
          itr->pnewentry.symbol != price_newentry.symbol ) {
      itr++;
    }

    if( itr != codeidx.end() &&
        itr->contract == contract &&
        itr->pnewentry.symbol == price_newentry.symbol ) {
      _prices.modify( *itr, _self, [&]( auto& p ) {
          p.pnewentry = price_newentry;
          p.psubentry = price_subentry;
        });
    } else {
      _prices.emplace(_self, [&]( auto& p ) {
          p.id = _prices.available_primary_key();
          p.contract = contract;
          p.pnewentry = price_newentry;
          p.psubentry = price_subentry;
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

  
 
  void set_entry_tags(const ENTRY& e, const vector<name>& tags)
  {
    eosio_assert(tags.size() <= MAX_TAGS, "Too many tags");
    auto entitr = get_entry(e);
    auto entry_id = entitr->id;
    
    // delete all tags for an entry
    auto tagidx = _tagcloud.template get_index<N(entryid)>();
    auto tagitr = tagidx.lower_bound(entry_id);
    while(tagitr != tagidx.end() && tagitr->entry_id == entry_id ) {
      tagitr = tagidx.erase(tagitr);
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
  void claimrefund(account_name owner)
  {
    require_owner_or_delegate_auth(owner);
    eosio_assert( !has_open_refund(owner),
                  "An unpaid refund is pending for this owner" );

    bool found = false;
    // erase tagcloud and entries
    auto entidx = _entries.template get_index<N(owner)>();
    auto entitr = entidx.lower_bound(owner);
    while(entitr != entidx.end() && entitr->owner == owner) {
      uint64_t entry_id = entitr->id;

      auto tagidx = _tagcloud.template get_index<N(entryid)>();
      auto tagitr = tagidx.lower_bound(entry_id);
      while(tagitr != tagidx.end() && tagitr->entry_id == entry_id ) {
        tagitr = tagidx.erase(tagitr);
      }

      entitr = entidx.erase(entitr);
      found = true;
    }

    eosio_assert(found, "No data found for this owner");

    // undelegate
    auto repsitr = _reps.find(owner);
    if( repsitr != _reps.end() ) {
      _reps.erase(repsitr);
    }

    // mark payments as refund pending
    auto payidx = _payments.template get_index<N(owner)>();
    auto payitr = payidx.lower_bound(owner);
    while(payitr != payidx.end() && payitr->owner == owner ) {
      _payments.modify( *payitr, _self, [&]( auto& p ) {
          p.torefund = true;
          p.balance.amount *= REFUND_RATE;
        });
      payitr++;
    }
   
    // perform payback automatically
    if( autorefund ) {
      payitr = payidx.lower_bound(owner);
      while(payitr != payidx.end() && payitr->owner == owner ) {
        action
          {
            permission_level{_self, N(active)},
              payitr->balance.contract,
                N(transfer),
                currency::transfer
                  {
                    .from=_self, .to=owner,
                      .quantity=payitr->balance, .memo="Refund as requested"
                      }
          }.send(); 
        payitr++;
      }
    }
  }

  
  
  void refund(account_name to, const extended_asset& payment)
  {
    require_auth(_self);

    auto payidx = _payments.template get_index<N(owner)>();
    auto payitr = payidx.lower_bound(to);
    while(payitr != payidx.end() && payitr->owner == to ) {
      if( payitr->balance.contract == payment.contract &&
          payitr->balance.symbol == payment.symbol ) {
          
        eosio_assert(payitr->balance == payment || payitr->balance.amount == 0,
                     "Transfer amount is not the same as due to refund");
        payitr = payidx.erase(payitr);
      }
      else {
        payitr++;
      }
    }
  }      

  
  /// @abi action
  void delegate(account_name owner, account_name representative)
  {
    require_owner_or_delegate_auth(owner);
    auto repsitr = _reps.find(owner);
    if( repsitr == _reps.end() ) {
      eosio_assert(representative != owner, "Representative is the same as owner");
      _reps.emplace(_self, [&]( auto& d ) {
          d.owner = owner;
          d.representative = representative;
        });
    }
    else {
      if( representative == owner ) { // remove existimng representative
        _reps.erase(repsitr);
      }
      else {
        eosio_assert(repsitr->representative != representative,
                     "This representative is already set up for this owner");
        _reps.modify(*repsitr, _self, [&](auto& d) {
            d.representative = representative;});
      }
    }
  }
  

  void require_owner_or_delegate_auth(account_name owner)
  {
    if( has_auth(owner) ) {
      return;
    }

    auto repsitr = _reps.find(owner);
    if( repsitr == _reps.end() ) {
      require_auth(owner); // throw a standart exception with standart text
    } else {
      require_auth(repsitr->representative);
    }
  }
      

private:

  /// @abi table
  struct price {
    uint64_t       id;
    account_name   contract;
    asset          pnewentry;
    asset          psubentry;
    auto primary_key()const { return id; }
    uint64_t get_contract()const { return contract; }
  };
  

  typedef eosio::multi_index<N(price), price,
    indexed_by<N(contract), const_mem_fun<price, uint64_t, &price::get_contract>>> prices;

  const price& get_price_obj(const account_name contract, const symbol_type symbol)
  {
    auto contractidx = _prices.template get_index<N(contract)>();
    auto itr = contractidx.lower_bound(contract);
    eosio_assert(itr != contractidx.end(), "Unknown currency issuer");
    while( itr->contract == contract ) {
      if( itr->pnewentry.symbol == symbol ) {
        return *itr;
      }
      itr++;
    }
    eosio_assert(0, "Unknown currency name");
    return *itr;
  }

  auto get_entry(const ENTRY& ent)
  {
    auto entidx = _entries.template get_index<N(owner)>();
    auto entitr = entidx.lower_bound(ent.owner);
    eosio_assert( entitr != entidx.end(), "Cannot find entry for this owner");
                  
    while(entitr != entidx.end() && entitr->owner == ent.owner && *entitr != ent ) {
      entitr++;
    }

    eosio_assert(*entitr == ent, "Cannot find such item in entry table");
    return entitr;
  }


  /// @abi table
  struct payment {
    uint64_t       id;
    account_name   owner;
    extended_asset balance;
    bool           torefund = false;
    auto primary_key()const { return id; }
    uint64_t get_owner()const { return owner; }
    uint64_t get_refund()const { return torefund?1:0; }
  };

  typedef eosio::multi_index<N(payment), payment,
    indexed_by<N(owner), const_mem_fun<payment, uint64_t, &payment::get_owner>>,
    indexed_by<N(refund), const_mem_fun<payment, uint64_t, &payment::get_refund>>> payments;

  
  void register_payment(account_name owner, const extended_asset& payment)
  {
    auto payidx = _payments.template get_index<N(owner)>();
    auto payitr = payidx.lower_bound(owner);
    while(payitr != payidx.end() &&
          payitr->owner == owner &&
          payitr->balance.contract != payment.contract &&
          payitr->balance.symbol != payment.symbol ) {
      payitr++;
    }

    if( payitr != payidx.end() &&
        payitr->owner == owner &&
        payitr->balance.contract == payment.contract &&
        payitr->balance.symbol == payment.symbol ) {
      _payments.modify( *payitr, _self, [&]( auto& p ) {
          p.balance += payment;
        });
    } else {
      _payments.emplace(_self, [&]( auto& p ) {
          p.id = _payments.available_primary_key();
          p.owner = owner;
          p.balance = payment;
        });
    }
  }

  
  bool has_open_refund(account_name owner)
  {
    auto payidx = _payments.template get_index<N(owner)>();
    auto payitr = payidx.lower_bound(owner);
    while(payitr != payidx.end() && payitr->owner == owner  ) {
      if( payitr->torefund ) {
        return true;
      }
      
      payitr++;
    }
    return false;
  }


  
  /// @abi table
  struct rep {
    account_name       owner;
    account_name       representative;
    auto primary_key()const { return owner; }
  };

  typedef eosio::multi_index<N(rep), rep> reps;
  
  
  prices _prices;
  tagcloud _tagcloud;
  entries _entries;
  payments _payments;
  reps _reps;
};


  
  
