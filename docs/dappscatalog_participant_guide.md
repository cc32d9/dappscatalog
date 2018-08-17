# `dappscatalog` Participant's Guide

`dappscatalog` is a smart contract in EOS mainnet that is designed to
store and present information about smart contracts and tokens in the
EOS network.

Authenticuty of all data records is guaranteed by the contract owners'
private key signatures. The data is available nn the blockchain and also
browsable via http://www.dappscatalog.com website.


## Record format


Each data record has a number of fields as follows:

* `id`: unique record identifier

* `owner`: account name of the smart contract owning this record

* `tokenname`: empty string or token name

* `title`: human-readable title, like application name

* `org`: organization name

* `urlwebsite`: URL pointing to the project website

* `urllogo`: URL pointing to the logo image that would be displayed on
  dappscatalog website

* `urlsrccode`: URL pointing to a folder in GitHub project or some other
  source repository

* `email`: contact e-mail address

* `descr`: project description, up to 1024 characters

* `flags`: an integer with two lower bits indicating completeness and
  listing of the entry


## Adding a record


New entries are added when a contract owner performs a payment to
`dappscatalog` account. It is important that the payment comes from the
smart contract account. The payer account name then appears as `owner`
field, and it not mutable. You can later delegate the rights to modify
the entry to another account if you need to.

A smart contract accout may have multiple entries in `dappscatalog`
database, indicating different token names. In this case, subsequent
entries are added at a lower price.

If the smart contract does not issue any tokens, the `tokenname` field
can be left empty.

At the time of record creation, the payment is expected to have the
token name in memo text, or come with empty memo.

If the payment amount is incorrect, `dappscatalog` rejects the payment,
and expected amount is reported in the exception message.


## Pricing


* First entry for an owner: "100.0000 EOS".

* Subsequent entries for the same owner: "20.0000 EOS"

* This payment is a one-time fee for the life time of the
  blockchain. Also all required RAM is paid by `dappscatalog`.

* Promotion during the project launch: "20.0000 EOS" for the first 10
  payers.

* The owners of `dappscatalog` may issue discount vouchers to some
  projects.

The owner of an entry can claim a refund, and `dappscatalog` will erase
all the owner's entries and send back 90% of all paid amounts.

There is only one-time payment for creating the record. All
modifications on the record contents are free of charge, and the record
will stay for as long as EOS mainnet is operational.


## Tags and attributes

Each record can have up to 10 tags and up to 5 attributes.

Tags will form a tag cloud, so that your users would easily find your
entries. Tag names are up to 12 characters long.

Attributes are arbitrary key-value pairs that will be displayed at
dappscatalog.com website. Each key name is up to 12 symbols long, and
values are up to 80 characters long.


## Flags

Each entry has a `flags` integer field which indicates the status of the
entry. The first two bits are for completeness and visibility, so the
value "3" indicates that the entry is complete and ready for displaying.

The third bit indicates blacklisted owner. The administrator of
dappscatalog reserves the right to blacklist accounts for inappropriate
content.



## Quick start example


Here `lunarcrawler` is an example account that owns a smart contract,
and this contract issues a token called LUNAR. The examples assume using
`cleos` command-line utility from EOS.io software suite. It is also
possible to use other tools like Scatter.

```
# create a new entry
cleos -u https://api.eosnewyork.io transfer lunarcrawler dappscatalog \
 "100.0000 EOS" LUNAR

# Set the record fields
cleos -u https://api.eosnewyork.io push action dappscatalog setvalue \
 '["lunarcrawler", "LUNAR", "title", "Lunar Crawler"]' -p lunarcrawler

cleos -u https://api.eosnewyork.io push action dappscatalog setvalue \
 '["lunarcrawler", "LUNAR", "org", "Lunar Crawler, Inc."]' -p lunarcrawler

cleos -u https://api.eosnewyork.io push action dappscatalog setvalue \
 '["lunarcrawler", "LUNAR", "urlwebsite", "https://lunarcrawler.io"]' -p lunarcrawler

cleos -u https://api.eosnewyork.io push action dappscatalog setvalue \
 '["lunarcrawler", "LUNAR", "urllogo", "https://lunarcrawler.io/logo.gif"]' -p lunarcrawler

cleos -u https://api.eosnewyork.io push action dappscatalog setvalue \
 '["lunarcrawler", "LUNAR", "urlsrccode", "https://github.com/lunarcrawler/lunarcrawler/tree/master/contract"]' -p lunarcrawler

cleos -u https://api.eosnewyork.io push action dappscatalog setvalue \
 '["lunarcrawler", "LUNAR", "email", "groundcontrol@lunarcrawler.io"]' -p lunarcrawler

cleos -u https://api.eosnewyork.io push action dappscatalog setvalue \
 '["lunarcrawler", "LUNAR", "descr", "An awesome lunar expedition"]' -p lunarcrawler


# Set the tags for the tag cloud
cleos -u https://api.eosnewyork.io push action dappscatalog settags \
 '["lunarcrawler", "LUNAR", ["game", "space", "geek"]]' -p lunarcrawler

# Set attributes
cleos -u https://api.eosnewyork.io push action dappscatalog setattr \
 '["lunarcrawler", "LUNAR", "status", "production"]' -p lunarcrawler
cleos -u https://api.eosnewyork.io push action dappscatalog setattr \
 '["lunarcrawler", "LUNAR", "telegram", "@lunarcrawler"]' -p lunarcrawler


# Set the record as ready for listing
cleos -u https://api.eosnewyork.io push action dappscatalog setflag \
 '["lunarcrawler", "LUNAR", "ready"]' -p lunarcrawler

# Allow the account "spaceodddity" act on behalf of "lunarcrawler" and edit the entries
cleos -u https://api.eosnewyork.io push action dappscatalog delegate \
 '["lunarcrawler", "spaceodddity"]' -p lunarcrawler

# Retrieve our record from the catalog
cleos -u https://api.eosnewyork.io get table dappscatalog dappscatalog entry \
 --key-type name --index 2 --lower lunarcrawler --limit 1
```


## ABI documentation (record owner's actions)

### ACTION NAME: setvalue

The action modifies one field in the record.

Input parameters:

* `owner` (entry owner's account name)
* `tokenname` (token name or emty string)
* `key` (record field name, such as title, org, urlwebsite, urllogo, urlsrccode, email, descr)
* `value` (string value for a field, up to 1024 characters)


### ACTION NAME: setvalues

The action modifies several fields in the record at the same time.

Input parameters:

* `owner` (entry owner's account name)
* `tokenname` (token name or emty string)
* `values` (array of (key,value) pairs. Key names are the same as in setvalue action)


### ACTION NAME: settags

The action replaces the tags for the record with a new list of tags.

Input parameters:

* `owner` (entry owner's account name)
* `tokenname` (token name or emty string)
* `tags` (array of tag names)


### ACTION NAME: modtokenname

The action modifies or adds or removes the token name from the record.

Input parameters:

* `owner` (entry owner's account name)
* `oldtokenname` (token name or emty string)
* `newtokenname` (token name or emty string)


### ACTION NAME: delegate

The action allows another account to be the owner's representative and
modify all their records, as well as claim a refund. The refund would be
paid to the original owner.

If the second parameter is the same account name as the owner, the old
representative right is removed from the system.

Input parameters:

* `owner` (entry owner's account name)
* `representative` (representative's account name)


### ACTION NAME: claimrefund

The action erases all records of the owner and pays back 90% of paid amount.

Input parameters:

* `owner` (entry owner's account name)



## ABI documentation (`dappscatalog` administration)

### ACTION NAME: setprice

The action sets up prices for new and subsequent entries in a specified
currency. There may be prices for multiple currencies.

Input parameters:

* `contract` (token contract name)
* `price_newentry` (standard price for the first entry in the catalog)
* `price_subentry` (standard price for additional entry for the same owner)


### ACTION NAME: startpromo

The action initiates a promotion price for a specified number of new
payers. There may be several different promotions in different
currencies.

Input parameters:

* `contract` (token contract name)
* `price` (promotion price for the first entry in the catalog)
* `count` (number of new entries offered at promotion price)


### ACTION NAME: addvoucher

The action sets up a personalized voucher for a future owner of a
record. Claiming of the voucher is performed by payment from the
specified owner account.

Input parameters:

* `owner` (entry owner's account name)
* `contract` (token contract name)
* `price` (promotion price for the first entry in the catalog)


### ACTION NAME: remvoucher

The action removes a personalized voucher from the system.

Input parameters:

* `owner` (entry owner's account name)


## Souce code, license and copyright

Source code repository: https://github.com/cc32d9/eos.dappscatalog

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








