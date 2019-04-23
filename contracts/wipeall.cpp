/*
Copyright 2018 cc32d9@gmail.com

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/


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

  [[eosio::action]]
  void wipeall()
  {
    require_auth( _self );

    {
      auto itr = _prices.begin();
      while( itr != _prices.end() ) {
        itr = _prices.erase(itr);
      }
    }

    {
      auto itr = _vouchers.begin();
      while( itr != _vouchers.end() ) {
        itr = _vouchers.erase(itr);
      }
    }
            
    {
      auto itr = _tagcloud.begin();
      while( itr != _tagcloud.end() ) {
        itr = _tagcloud.erase(itr);
      }
    }

    {
      auto itr = _attrs.begin();
      while( itr != _attrs.end() ) {
        itr = _attrs.erase(itr);
      }
    }

    {
      auto itr = _entries.begin();
      while( itr != _entries.end() ) {
        itr = _entries.erase(itr);
      }
    }

    {
      auto itr = _payments.begin();
      while( itr != _payments.end() ) {
        itr = _payments.erase(itr);
      }
    }

    {
      auto itr = _reps.begin();
      while( itr != _reps.end() ) {
        itr = _reps.erase(itr);
      }
    }
    
    {
      auto itr = _props.begin();
      while( itr != _props.end() ) {
        itr = _props.erase(itr);
      }
    }
  }
  
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


EOSIO_DISPATCH( dappscatalog, (wipeall));




