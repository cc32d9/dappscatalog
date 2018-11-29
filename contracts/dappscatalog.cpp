#include <eosiolib/eosio.hpp>
#include <eosiolib/action.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/multi_index.hpp>



using namespace eosio;
using std::string;


class [[eosio::contract]] dappscatalog : public eosio::contract {
 public:
  dappscatalog( name self, name code, datastream<const char*> ds ):
    contract(self, code, ds), _tagcloud(self, self.value),
    _attrs(self, self.value), _prices(self, self.value),
    _entries(self, self.value), _payments(self, self.value), _reps(self, self.value),
    _vouchers(self, self.value), _props(self, self.value)
    {}
  
  const int MAX_VALUE_LEN = 1024;
  const float REFUND_RATE = 0.9;
  const bool autorefund = true;
  const int MAX_TAGS = 10;
  const int MAX_ATTRS = 5;
  const int MAX_ATTR_LEN = 80;

  struct [[eosio::table("entries")]] entry {
    uint64_t      id;
    name          owner;
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
    uint64_t get_owner()const { return owner.value; }
    
    bool operator==( const entry& b ) const {
      return (this->owner == b.owner && this->tokenname == b.tokenname);
    }

    bool operator!=( const entry& b ) const {
      return (this->owner != b.owner || this->tokenname != b.tokenname);
    }
  }; 

  typedef eosio::multi_index<name("entries"), entry,
    indexed_by<name("owner"), const_mem_fun<entry, uint64_t, &entry::get_owner>>> entries;
  
  auto get_entry(const entry& ent)
  {
    auto entidx = _entries.get_index<name("owner")>();
    auto entitr = entidx.lower_bound(ent.owner.value);
    eosio_assert( entitr != entidx.end(), "Cannot find entry for this owner");
                  
    while(entitr != entidx.end() && entitr->owner == ent.owner && *entitr != ent ) {
      entitr++;
    }

    eosio_assert(*entitr == ent, "Cannot find such item in entry table");
    return entitr;
  }

  
  void transfer_action (name from, name to, asset quantity, string memo)
  {
    eosio_assert(quantity.is_valid(), "Invalid quantity");
    extended_asset payment(quantity, get_code());
    
    if(from == _self) {
      register_refund(to, payment);
      return;
    }

    if(to == _self) {
      verify_symbol_str(memo);
      entry e;
      e.owner = from;
      e.tokenname = memo;
      pay_add_entry(payment, e);
    }
  }
  

  [[eosio::action]]
  void setvalue (name owner, string tokenname, name key, string value)
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

  
  [[eosio::action]]
  void setvalues (name owner, string tokenname, std::vector<keyval> values)
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
  
  
  [[eosio::action]]
  void settags (name owner, string tokenname, std::vector<name>& tags)
  {
    require_owner_or_delegate_auth( owner );
    entry e;
    e.owner = owner;
    e.tokenname = tokenname;
    set_entry_tags( e, tags );
  }


  [[eosio::action]]
  void setattr (name owner, string tokenname, name key, const string val)
  {
    require_owner_or_delegate_auth( owner );
    entry e;
    e.owner = owner;
    e.tokenname = tokenname;
    set_attr( e, key, val );
  }

  [[eosio::action]]
  void delattr (name owner, string tokenname, name key)
  {
    require_owner_or_delegate_auth( owner );
    entry e;
    e.owner = owner;
    e.tokenname = tokenname;
    del_attr( e, key );
  }
  
  [[eosio::action]]
  void setflag (name owner, string tokenname, name flag)
  {
    require_owner_or_delegate_auth( owner );
    entry e;
    e.owner = owner;
    e.tokenname = tokenname;
    set_entry_flag( e, flag );
  }

  [[eosio::action]]
  void modtokenname (name owner, string oldtokenname, string newtokenname)
  {
    require_owner_or_delegate_auth( owner );    
    verify_symbol_str(newtokenname);

    entry e;
    e.owner = owner;
    e.tokenname = newtokenname;
    eosio_assert( !entry_exists(e), "Such token name already exists");

    e.tokenname = oldtokenname;
    modify_entry( e, [&]( auto& ent ) {
        ent.tokenname = newtokenname;
      });
  }

  [[eosio::action]]
  void blacklist(name owner)
  {
    require_auth( _self );
    entry e;
    e.owner = owner;
    e.title = "Blacklisted";
    set_blacklist_entry(e);
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
    switch(key.value) {
    case name("title").value:   ent.title = value;
      break;
    case name("org").value:     ent.org = value;
      break;
    case name("urlwebsite").value: ent.urlwebsite = value;
      break;
    case name("urllogo").value: ent.urllogo = value;
      break;
    case name("urlsrccode").value: ent.urlsrccode = value;
      break;
    case name("email").value:   ent.email = value;
      break;
    case name("descr").value:   ent.descr = value;
      break;
    default:
      eosio_assert(0, "Unknown key name");
    }
  }


  void check_blacklist( name owner )
  {
    auto entidx = _entries.get_index<name("owner")>();
    auto entitr = entidx.lower_bound(owner.value);
    if( entitr != entidx.end() && entitr->owner == owner ) {
      eosio_assert( (entitr->flags & 4UL) == 0, "This accouunt is blacklisted" );
    }
  }
    

  
  bool entry_exists( const entry& ent )
  {
    auto entidx = _entries.get_index<name("owner")>();
    auto entitr = entidx.lower_bound(ent.owner.value);
    while(entitr != entidx.end() && entitr->owner == ent.owner) {
      if( *entitr == ent ) {
        return true;
      }
      entitr++;
    }
    return false;
  }


  
  void pay_add_entry( const extended_asset& payment, const entry& ent )
  {
    check_blacklist( ent.owner );
    eosio_assert( !has_open_refund(ent.owner),
                  "An unpaid refund is pending for this owner" );

    asset exactprice;
    auto priceobj = get_price_obj(payment.contract, payment.quantity.symbol);
    
    auto entidx = _entries.get_index<name("owner")>();
    auto entitr = entidx.lower_bound(ent.owner.value);
    bool found = false;
    while(entitr != entidx.end() && entitr->owner == ent.owner) {
      found = true;
      eosio_assert(*entitr != ent, "The same entry already exists");
      entitr++;
    }

    if( found ) {
      exactprice = priceobj.psubentry;
    }
    else {
      auto vchr = _vouchers.find(ent.owner.value);
      if( vchr != _vouchers.end() ) {
        eosio_assert(payment.contract == vchr->contract,
                     "You have a voucher in different currency");
        exactprice = vchr->price;
        _vouchers.erase(vchr);
      }
      else if( priceobj.promocount > 0 ) {
        exactprice = priceobj.ppromo;
        auto pitr = _prices.find(priceobj.id);
        _prices.modify( *pitr, _self, [&]( auto& p ) {
            p.promocount--;
            if( p.promocount == 0 ) {
              p.ppromo.amount = 0;
            }
          });
      }
      else {
        exactprice = priceobj.pnewentry;
      }
    }

    // check if the payment amount is right

    if( payment.quantity != exactprice ) {
      int64_t pres = (int64_t)exactprice.symbol.precision();
      char buf[64];
      char* start = buf + sizeof(buf) - 10;
      char* end = start;
      int digits = 0;
      
      int64_t amount = exactprice.amount;
      while( amount != 0 || digits < pres+1 ) {
        --start;
        *start = (amount % 10) + '0';
        digits++;
        if( digits == pres ) {
          *--start = '.';
        }
        amount /= 10;
      }
      
      *end++ = ' ';
      uint64_t sym = exactprice.symbol.code().raw();
      for( int i = 0; i < 7; ++i ) {
        char c = (char)(sym & 0xff);
        *end++ = c;
        if( c == 0 ) break;
        sym >>= 8;
      }
      *end = 0;
      
      char str[256] = "Incorrect payment amount. Expected ";
      strcat(str, start);
      eosio_assert(0, str);
    }
    
    _entries.emplace(_self, [&]( auto& ce ) {
        ce = ent;
        ce.id = _entries.available_primary_key();
      });

    // remember the payment for possible refunding
    register_payment(ent.owner, payment);
    incr_revision();
  }


  void set_blacklist_entry( const entry& ent )
  {
    check_blacklist( ent.owner );
    erase_and_send_refund( ent.owner );
    _entries.emplace(_self, [&]( auto& ce ) {
        ce = ent;
        ce.id = _entries.available_primary_key();
        ce.flags = 4UL;
      });
  }
    



  
  template<typename Lambda>
  void modify_entry(const entry& e, Lambda&& updater )
  {
    auto entitr = get_entry(e);
    _entries.modify( *entitr, _self, std::forward<Lambda&&>(updater) );
    incr_revision();
  }

  
  void set_entry_flag(const entry& e, name flag)
  {
    auto entitr = get_entry(e);
    _entries.modify( *entitr, _self, [&]( auto& ent ) {
        switch(flag.value) {
        case name("ready").value: ent.flags |= 3UL;
          break;
        case name("complete").value: ent.flags |= 1UL;
          break;
        case name("incomplete").value: ent.flags &= ~(1UL);
          break;
        case name("show").value: ent.flags |= 2UL;
          break;
        case name("hide").value: ent.flags &= ~(2UL);
          break;
        default: eosio_assert(0, "Wrong flag name. Expected ready|complete|incomplete|show|hide");
        }});
    incr_revision();
  }    

  
  [[eosio::action]]
  void setprice(name contract,
                const asset& price_newentry,
                const asset& price_subentry)
  {
    require_auth( _self );
    eosio_assert(price_newentry.symbol == price_subentry.symbol,
                 "Newentry and subentry prices are in different currency");
    auto codeidx = _prices.get_index<name("contract")>();
    auto itr = codeidx.lower_bound(contract.value);
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


  [[eosio::action]]
  void startpromo(name contract, const asset& price, uint64_t count)
  {
    require_auth( _self );
    auto codeidx = _prices.get_index<name("contract")>();
    auto itr = codeidx.lower_bound(contract.value);
    while(itr != codeidx.end() &&
          itr->contract == contract &&
          itr->pnewentry.symbol != price.symbol ) {
      itr++;
    }

    eosio_assert((itr != codeidx.end() &&
                  itr->contract == contract &&
                  itr->pnewentry.symbol == price.symbol),
                 "Cannot find a price entry with such code and sumbol");

    _prices.modify( *itr, _self, [&]( auto& p ) {
        p.ppromo = price;
        p.promocount = count;
      });
  }


  [[eosio::action]]
  void addvoucher(name owner, name contract, const asset& price)
  {
    require_auth( _self );
    check_blacklist( owner );
    eosio_assert( _vouchers.find(owner.value) == _vouchers.end(),
                  "This owner has already got a voucher" );

    auto entidx = _entries.get_index<name("owner")>();
    auto entitr = entidx.lower_bound(owner.value);
    eosio_assert(entitr == entidx.end() || entitr->owner != owner,
                 "This owner has already got an entry" );
    
    _vouchers.emplace(_self, [&]( auto& v ) {
        v.owner = owner;
        v.contract = contract;
        v.price = price;
      });
    require_recipient( owner );
  }

    
  [[eosio::action]]
  void remvoucher(name owner)
  {
    require_auth( _self );
    auto itr = _vouchers.find(owner.value);
    eosio_assert( itr != _vouchers.end(),  "Cannot find a voucher for this owner" );
    _vouchers.erase(itr);
    require_recipient( owner );
  }

  
    
  struct [[eosio::table("tags")]] tag {
    uint64_t       id;
    uint64_t       entry_id;
    name           tag;
    auto primary_key()const { return id; }
    uint64_t get_tag()const { return tag.value; }
    uint64_t get_entry_id()const { return entry_id; }
  };

  typedef eosio::multi_index<name("tags"), tag,
    indexed_by<name("tag"), const_mem_fun<tag, uint64_t, &tag::get_tag>>,
    indexed_by<name("entryid"), const_mem_fun<tag, uint64_t, &tag::get_entry_id>>> tagcloud;

  
 
  void set_entry_tags(const entry& e, const std::vector<name>& tags)
  {
    eosio_assert(tags.size() <= MAX_TAGS, "Too many tags");
    auto entitr = get_entry(e);
    auto entry_id = entitr->id;
    
    // delete all tags for an entry
    auto tagidx = _tagcloud.get_index<name("entryid")>();
    auto tagitr = tagidx.lower_bound(entry_id);
    while(tagitr != tagidx.end() && tagitr->entry_id == entry_id ) {
      tagitr = tagidx.erase(tagitr);
    }
    
    for( name tag : tags ) {      
      _tagcloud.emplace(_self, [&]( auto& tg ) {
          tg.id = _tagcloud.available_primary_key();
          tg.entry_id = entry_id;
          tg.tag = tag;
        });
    }
    incr_revision();
  }


  struct [[eosio::table("attrs")]] attr {
    uint64_t       id;
    uint64_t       entry_id;
    name           key;
    string         value;
    auto primary_key()const { return id; }
    uint64_t get_entry_id()const { return entry_id; }
  };

  typedef eosio::multi_index<name("attrs"), attr,
    indexed_by<name("entryid"), const_mem_fun<attr, uint64_t, &attr::get_entry_id>>> attrs;

  
  void set_attr(const entry& e, name key, const string val)
  {
    eosio_assert(val.length() > 0, "Value cannot be empty");
    eosio_assert(val.length() <= MAX_ATTR_LEN, "Value is too long");
    auto entitr = get_entry(e);
    auto entry_id = entitr->id;
    
    incr_revision();
    auto attridx = _attrs.get_index<name("entryid")>();
    auto attritr = attridx.lower_bound(entry_id);
    int count = 0;
    while(attritr != attridx.end() && attritr->entry_id == entry_id ) {
      if( attritr->key == key ) {
        _attrs.modify( *attritr, _self, [&]( auto& a ) {
            a.value = val;
          });
        return;
      }
      count++;
      attritr++;
    }

    eosio_assert(count < MAX_ATTRS, "Too many attributes");
    _attrs.emplace(_self, [&]( auto& a ) {
        a.id = _attrs.available_primary_key();
        a.entry_id = entry_id;
        a.key = key;
        a.value = val;
      });
  }


  void del_attr(const entry& e, name key)
  {
    auto entitr = get_entry(e);
    auto entry_id = entitr->id;
    
    incr_revision();
    auto attridx = _attrs.get_index<name("entryid")>();
    auto attritr = attridx.lower_bound(entry_id);
    while(attritr != attridx.end() && attritr->entry_id == entry_id ) {
      if( attritr->key == key ) {
        attridx.erase(attritr);
        return;
      }
      attritr++;
    }
    eosio_assert(false, "Attribute not found");
  }

  
  
  [[eosio::action]]
  void claimrefund(name owner)
  {
    require_owner_or_delegate_auth(owner);
    erase_and_send_refund(owner);
  }

  
  
  void register_refund(name to, const extended_asset& payment)
  {
    require_auth(_self);

    auto payidx = _payments.get_index<name("owner")>();
    auto payitr = payidx.lower_bound(to.value);
    while(payitr != payidx.end() && payitr->owner == to ) {
      if( payitr->balance.contract == payment.contract &&
          payitr->balance.quantity.symbol == payment.quantity.symbol ) {
          
        eosio_assert(payitr->balance == payment || payitr->balance.quantity.amount == 0,
                     "Transfer amount is not the same as due to refund");
        payitr = payidx.erase(payitr);
      }
      else {
        payitr++;
      }
    }
  }      

  
  [[eosio::action]]
  void delegate(name owner, name representative)
  {
    require_owner_or_delegate_auth(owner);
    auto repsitr = _reps.find(owner.value);
    if( repsitr == _reps.end() ) {
      eosio_assert(representative != owner, "Representative is the same as owner");
      _reps.emplace(_self, [&]( auto& d ) {
          d.owner = owner;
          d.representative = representative;
        });
      require_recipient( representative );
    }
    else {
      if( representative == owner ) { // remove existimng representative
        require_recipient( repsitr->representative );
        _reps.erase(repsitr);
      }
      else {
        eosio_assert(repsitr->representative != representative,
                     "This representative is already set up for this owner");
        require_recipient( repsitr->representative );        
        require_recipient( representative );        
        _reps.modify(*repsitr, _self, [&](auto& d) {
            d.representative = representative;});
      }
    }
  }
  

  void require_owner_or_delegate_auth(name owner)
  {
    check_blacklist(owner);
    if( has_auth(owner) ) {
      return;
    }

    auto repsitr = _reps.find(owner.value);
    if( repsitr == _reps.end() ) {
      require_auth(owner); // throw a standart exception with standart text
    } else {
      require_auth(repsitr->representative);
    }
  }

  
  void setprop_incr(name prop)
  {
    auto propitr = _props.find(prop.value);
    if( propitr != _props.end() ) {
      _props.modify( *propitr, _self, [&]( auto& p ) {
          p.val_uint++;
        });
    }
    else {
      _props.emplace(_self, [&]( auto& p ) {
          p.property = prop;
          p.val_uint = 1;
          p.val_str = "";
          p.val_name.value = 0;
        });
    }
  }        

  inline void incr_revision()
  {
    setprop_incr(name("revision"));
  }

  
private:

  struct [[eosio::table("prices")]] price {
    uint64_t       id;
    name           contract;
    asset          pnewentry;
    asset          psubentry;
    asset          ppromo;
    uint64_t       promocount = 0;
    auto primary_key()const { return id; }
    uint64_t get_contract()const { return contract.value; }
  };
  

  typedef eosio::multi_index<name("prices"), price,
    indexed_by<name("contract"), const_mem_fun<price, uint64_t, &price::get_contract>>> prices;

  const price& get_price_obj(const name contract, const symbol sym)
  {
    auto contractidx = _prices.get_index<name("contract")>();
    auto itr = contractidx.lower_bound(contract.value);
    eosio_assert(itr != contractidx.end(), "Unknown currency issuer");
    while( itr->contract == contract ) {
      if( itr->pnewentry.symbol == sym ) {
        return *itr;
      }
      itr++;
    }
    eosio_assert(0, "Unknown currency name");
    return *itr;
  }


  struct [[eosio::table("vouchers")]] voucher {
    name           owner;
    name           contract;
    asset          price;
    auto primary_key()const { return owner.value; }
  };

  typedef eosio::multi_index<name("vouchers"), voucher> vouchers;
  


  struct [[eosio::table("payments")]] payment {
    uint64_t       id;
    name           owner;
    extended_asset balance;
    bool           torefund = false;
    auto primary_key()const { return id; }
    uint64_t get_owner()const { return owner.value; }
    uint64_t get_refund()const { return torefund?1:0; }
  };

  typedef eosio::multi_index<name("payments"), payment,
    indexed_by<name("owner"), const_mem_fun<payment, uint64_t, &payment::get_owner>>,
    indexed_by<name("refund"), const_mem_fun<payment, uint64_t, &payment::get_refund>>> payments;

  
  void register_payment(name owner, const extended_asset& payment)
  {
    auto payidx = _payments.get_index<name("owner")>();
    auto payitr = payidx.lower_bound(owner.value);
    while(payitr != payidx.end() &&
          payitr->owner == owner &&
          payitr->balance.contract != payment.contract &&
          payitr->balance.quantity.symbol != payment.quantity.symbol ) {
      payitr++;
    }

    if( payitr != payidx.end() &&
        payitr->owner == owner &&
        payitr->balance.contract == payment.contract &&
        payitr->balance.quantity.symbol == payment.quantity.symbol ) {
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

  
  bool has_open_refund(name owner)
  {
    auto payidx = _payments.get_index<name("owner")>();
    auto payitr = payidx.lower_bound(owner.value);
    while(payitr != payidx.end() && payitr->owner == owner  ) {
      if( payitr->torefund ) {
        return true;
      }
      
      payitr++;
    }
    return false;
  }

  struct transfer
  {
    name         from;
    name         to;
    asset        quantity;
    string       memo;
  };

  void erase_and_send_refund(name owner)
  {
    eosio_assert( !has_open_refund(owner),
                  "An unpaid refund is pending for this owner" );

    bool found = false;
    // erase tagcloud and entries
    auto entidx = _entries.get_index<name("owner")>();
    auto entitr = entidx.lower_bound(owner.value);
    while(entitr != entidx.end() && entitr->owner == owner) {
      uint64_t entry_id = entitr->id;

      auto tagidx = _tagcloud.get_index<name("entryid")>();
      auto tagitr = tagidx.lower_bound(entry_id);
      while(tagitr != tagidx.end() && tagitr->entry_id == entry_id ) {
        tagitr = tagidx.erase(tagitr);
      }

      entitr = entidx.erase(entitr);
      found = true;
    }

    eosio_assert(found, "No data found for this owner");

    // undelegate and notify the delegate
    auto repsitr = _reps.find(owner.value);
    if( repsitr != _reps.end() ) {
      require_recipient( repsitr->representative );
      _reps.erase(repsitr);
    }

    // mark payments as refund pending
    auto payidx = _payments.get_index<name("owner")>();
    auto payitr = payidx.lower_bound(owner.value);
    while(payitr != payidx.end() && payitr->owner == owner ) {
      _payments.modify( *payitr, _self, [&]( auto& p ) {
          p.torefund = true;
          p.balance.quantity.amount *= REFUND_RATE;
        });
      payitr++;
    }
   
    // perform payback automatically
    if( autorefund ) {
      payitr = payidx.lower_bound(owner.value);
      while(payitr != payidx.end() && payitr->owner == owner ) {
        action
          {
            permission_level{_self, name("active")},
              payitr->balance.contract,
                name("transfer"),
                transfer
                  {
                    .from=_self, .to=owner,
                      .quantity=payitr->balance.quantity, .memo="Refund as requested"
                      }
          }.send(); 
        payitr++;
      }
    }

    incr_revision();
  }
  

  
  struct [[eosio::table("reps")]] rep {
    name       owner;
    name       representative;
    auto primary_key()const { return owner.value; }
  };

  typedef eosio::multi_index<name("reps"), rep> reps;
  

  struct [[eosio::table("props")]] prop {
    name         property;
    uint64_t     val_uint;
    string       val_str;
    name         val_name;
    auto primary_key()const { return property.value; }
  };

  typedef eosio::multi_index<name("props"), prop> props;

  
 private:
  prices _prices;
  vouchers _vouchers;
  tagcloud _tagcloud;
  attrs _attrs;
  entries _entries;
  payments _payments;
  reps _reps;
  props _props;
};


extern "C" void apply(uint64_t receiver, uint64_t code, uint64_t action) {
  if( action == name("transfer").value ) {
    execute_action<dappscatalog>( eosio::name(receiver), eosio::name(code),
                                  &dappscatalog::transfer_action );
  }
  else if( code == receiver ) {
    switch( action ) {
      EOSIO_DISPATCH_HELPER( dappscatalog,
                             (setprice)(setvalue)(setvalues)(settags)(setattr)(delattr)
                             (setflag)(modtokenname)(claimrefund)(delegate)(startpromo)
                             (addvoucher)(remvoucher)(blacklist));
    }                                       
  }
}




