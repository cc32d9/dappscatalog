

## Adding a new entry

```
# first entry payment
cleos -u http://rpc.eoswatch.info:9000 transfer tsttoken11 tokencatalog "25.0000 SYS" TESTA

# secondary entries are cheaper
cleos -u http://rpc.eoswatch.info:9000 transfer tsttoken11 tokencatalog "5.0000 SYS" TESTB

# Set the title string
cleos -u http://rpc.eoswatch.info:9000 push action tokencatalog setvalue '["tsttoken11", "TESTA", "title", "blah blah xxx"]' -p tsttoken11

# Set the tags for tag cloud
cleos -u http://rpc.eoswatch.info:9000 push action tokencatalog settags '["tsttoken11", "TESTA", ["gaga","gugu", "gogo"]]' -p tsttoken11


# all fields set up, ready to publish
cleos -u http://rpc.eoswatch.info:9000 push action tokencatalog setflag '["tsttoken11", "TESTA", "ready"]' -p tsttoken11

# something is wrong, hide from the listing
cleos -u http://rpc.eoswatch.info:9000 push action tokencatalog setflag '["tsttoken11", "TESTA", "hide"]' -p tsttoken11

# show again in the listing
cleos -u http://rpc.eoswatch.info:9000 push action tokencatalog setflag '["tsttoken11", "TESTA", "show"]' -p tsttoken11


# erase entries and claim money back
cleos -u http://rpc.eoswatch.info:9000 push action tokencatalog claimrefund '["tsttoken11"]' -p tsttoken11

```