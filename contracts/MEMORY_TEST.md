```
cleos -u http://rpc.eoswatch.info:9000 system newaccount --transfer --stake-net "100.0000 SYS" --stake-cpu "100.0000 SYS" --buy-ram-kbytes 1024 eosio tokencataxxx EOS8C9tb8QQhZet6WWcYFCWDKHYfjC3W59ugHCD63s7LLDQx6JsNK EOS8C9tb8QQhZet6WWcYFCWDKHYfjC3W59ugHCD63s7LLDQx6JsNK

```

```
$ cleos -u http://rpc.eoswatch.info:9000 get account tokencataxxx
permissions: 
     owner     1:    1 EOS8C9tb8QQhZet6WWcYFCWDKHYfjC3W59ugHCD63s7LLDQx6JsNK
        active     1:    1 EOS8C9tb8QQhZet6WWcYFCWDKHYfjC3W59ugHCD63s7LLDQx6JsNK
memory: 
     quota:      1019 KiB    used:     3.365 KiB  

net bandwidth: 
     staked:        100.0000 SYS           (total stake delegated from account to self)
     delegated:       0.0000 SYS           (total staked delegated to account from others)
     used:                 0 bytes
     available:         8.64 MiB  
     limit:             8.64 MiB  

cpu bandwidth:
     staked:        100.0000 SYS           (total stake delegated from account to self)
     delegated:       0.0000 SYS           (total staked delegated to account from others)
     used:                 0 us   
     available:        1.728 sec  
     limit:            1.728 sec  

producers:     <not voted>

```

```
cc32d9@ubuntu01:~/build/dappscatalog/contracts$ ls -al 
total 600
drwxrwxr-x 2 cc32d9 cc32d9   4096 Aug  1 11:39 .
drwxrwxr-x 4 cc32d9 cc32d9   4096 Jul 27 22:09 ..
-rw-rw-r-- 1 cc32d9 cc32d9   9997 Aug  1 09:47 catalog.hpp
-rw-rw-r-- 1 cc32d9 cc32d9     20 Jul 27 22:09 .gitignore
-rw-rw-r-- 1 cc32d9 cc32d9    194 Jul 27 22:26 Makefile
-rw-rw-r-- 1 cc32d9 cc32d9   1218 Aug  1 11:39 MEMORY_TEST.md
-rw-rw-r-- 1 cc32d9 cc32d9   1920 Aug  1 09:53 README.md
-rw-rw-r-- 1 cc32d9 cc32d9   5288 Aug  1 09:50 tokencatalog.abi
-rw-rw-r-- 1 cc32d9 cc32d9   4543 Aug  1 09:50 tokencatalog.cpp
-rw-rw-r-- 1 cc32d9 cc32d9  42577 Aug  1 09:50 tokencatalog.wasm
-rw-rw-r-- 1 cc32d9 cc32d9 508896 Aug  1 09:50 tokencatalog.wast
-rw-rw-r-- 1 cc32d9 cc32d9    635 Jul 28 09:45 unused_pieces.cpp

cleos -u http://rpc.eoswatch.info:9000 set contract tokencataxxx . tokencatalog.wast tokencatalog.abi
```

```
$ cleos -u http://rpc.eoswatch.info:9000 get account tokencataxxx
permissions: 
     owner     1:    1 EOS8C9tb8QQhZet6WWcYFCWDKHYfjC3W59ugHCD63s7LLDQx6JsNK
        active     1:    1 EOS8C9tb8QQhZet6WWcYFCWDKHYfjC3W59ugHCD63s7LLDQx6JsNK
memory: 
     quota:      1019 KiB    used:     420.1 KiB  

net bandwidth: 
     staked:        100.0000 SYS           (total stake delegated from account to self)
     delegated:       0.0000 SYS           (total staked delegated to account from others)
     used:             14.27 KiB  
     available:        8.626 MiB  
     limit:             8.64 MiB  

cpu bandwidth:
     staked:        100.0000 SYS           (total stake delegated from account to self)
     delegated:       0.0000 SYS           (total staked delegated to account from others)
     used:             10.99 ms   
     available:        1.717 sec  
     limit:            1.728 sec  

producers:     <not voted>
```

```
cleos -u http://rpc.eoswatch.info:9000 push action tokencataxxx setprice '[ "eosio.token", "25.0000 SYS", "5.0000 SYS" ]' -p tokencataxxx


cleos -u http://rpc.eoswatch.info:9000 get account tokencataxxx
permissions: 
     owner     1:    1 EOS8C9tb8QQhZet6WWcYFCWDKHYfjC3W59ugHCD63s7LLDQx6JsNK
        active     1:    1 EOS8C9tb8QQhZet6WWcYFCWDKHYfjC3W59ugHCD63s7LLDQx6JsNK
memory: 
     quota:      1019 KiB    used:     420.5 KiB  

net bandwidth: 
     staked:        100.0000 SYS           (total stake delegated from account to self)
     delegated:       0.0000 SYS           (total staked delegated to account from others)
     used:             14.37 KiB  
     available:        8.626 MiB  
     limit:             8.64 MiB  

cpu bandwidth:
     staked:        100.0000 SYS           (total stake delegated from account to self)
     delegated:       0.0000 SYS           (total staked delegated to account from others)
     used:             12.56 ms   
     available:        1.715 sec  
     limit:            1.728 sec  

producers:     <not voted>

```

```
cleos -u http://rpc.eoswatch.info:9000 transfer tsttoken11 tokencataxxx "25.0000 SYS" TESTA

cleos -u http://rpc.eoswatch.info:9000 get account tokencataxxx
permissions: 
     owner     1:    1 EOS8C9tb8QQhZet6WWcYFCWDKHYfjC3W59ugHCD63s7LLDQx6JsNK
        active     1:    1 EOS8C9tb8QQhZet6WWcYFCWDKHYfjC3W59ugHCD63s7LLDQx6JsNK
memory: 
     quota:      1019 KiB    used:     421.5 KiB  

net bandwidth: 
     staked:        100.0000 SYS           (total stake delegated from account to self)
     delegated:       0.0000 SYS           (total staked delegated to account from others)
     used:             14.37 KiB  
     available:        8.626 MiB  
     limit:             8.64 MiB  

cpu bandwidth:
     staked:        100.0000 SYS           (total stake delegated from account to self)
     delegated:       0.0000 SYS           (total staked delegated to account from others)
     used:             12.56 ms   
     available:        1.715 sec  
     limit:            1.728 sec  

SYS balances: 
     liquid:           25.0000 SYS
     staked:          200.0000 SYS
     unstaking:         0.0000 SYS
     total:           225.0000 SYS

producers:     <not voted>

```

```
cleos -u http://rpc.eoswatch.info:9000 transfer tsttoken12 tokencataxxx "25.0000 SYS" XTESTXA

cleos -u http://rpc.eoswatch.info:9000 get account tokencataxxx
permissions: 
     owner     1:    1 EOS8C9tb8QQhZet6WWcYFCWDKHYfjC3W59ugHCD63s7LLDQx6JsNK
        active     1:    1 EOS8C9tb8QQhZet6WWcYFCWDKHYfjC3W59ugHCD63s7LLDQx6JsNK
memory: 
     quota:      1019 KiB    used:     422.1 KiB  

net bandwidth: 
     staked:        100.0000 SYS           (total stake delegated from account to self)
     delegated:       0.0000 SYS           (total staked delegated to account from others)
     used:             14.37 KiB  
     available:        8.626 MiB  
     limit:             8.64 MiB  

cpu bandwidth:
     staked:        100.0000 SYS           (total stake delegated from account to self)
     delegated:       0.0000 SYS           (total staked delegated to account from others)
     used:             12.56 ms   
     available:        1.715 sec  
     limit:            1.728 sec  

SYS balances: 
     liquid:           50.0000 SYS
     staked:          200.0000 SYS
     unstaking:         0.0000 SYS
     total:           250.0000 SYS

producers:     <not voted>
```

```
cleos -u http://rpc.eoswatch.info:9000 transfer tsttoken12 tokencataxxx "5.0000 SYS" XTESTXB

cleos -u http://rpc.eoswatch.info:9000 get account tokencataxxx
permissions: 
     owner     1:    1 EOS8C9tb8QQhZet6WWcYFCWDKHYfjC3W59ugHCD63s7LLDQx6JsNK
        active     1:    1 EOS8C9tb8QQhZet6WWcYFCWDKHYfjC3W59ugHCD63s7LLDQx6JsNK
memory: 
     quota:      1019 KiB    used:     422.4 KiB  

net bandwidth: 
     staked:        100.0000 SYS           (total stake delegated from account to self)
     delegated:       0.0000 SYS           (total staked delegated to account from others)
     used:             14.37 KiB  
     available:        8.626 MiB  
     limit:             8.64 MiB  

cpu bandwidth:
     staked:        100.0000 SYS           (total stake delegated from account to self)
     delegated:       0.0000 SYS           (total staked delegated to account from others)
     used:             12.56 ms   
     available:        1.715 sec  
     limit:            1.728 sec  

SYS balances: 
     liquid:           55.0000 SYS
     staked:          200.0000 SYS
     unstaking:         0.0000 SYS
     total:           255.0000 SYS

producers:     <not voted>
```

```
cleos -u http://rpc.eoswatch.info:9000 push action tokencataxxx setvalues '["tsttoken11", "TESTA", [["title", "Lorem Ipsum"],["org", "Lorem Ipsomania, Inc."], ["url", "http://loremipsumloremipsum.org"], ["email", "lorem@ipsum.org"], ["descr", "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam cursus orci lorem, sit amet volutpat odio sagittis vel. Nulla volutpat sed augue euismod malesuada. Donec mi magna, cursus eu commodo in, malesuada vel augue. Etiam sagittis odio eu lorem mollis, non fringilla purus sodales. Curabitur a semper nulla, nec eleifend risus. Aenean venenatis lorem vulputate dignissim bibendum. Vestibulum bibendum finibus nibh in pellentesque. In porttitor condimentum enim, quis malesuada orci aliquam at. Integer ac est vel enim volutpat bibendum vitae a arcu. Etiam interdum tortor id mauris lobortis, eu mattis lectus gravida. Sed sollicitudin lectus dui, eget suscipit urna bibendum vitae. Sed in commodo lacus. Curabitur turpis felis, bibendum molestie lectus in, vehicula tristique ante. Ut tristique nisl orci, et euismod dolor bibendum ut. Ut imperdiet urna vitae libero pharetra, eget consequat sem porta. Duis pellentesque tempus posuere."]]]' -p tsttoken11

cleos -u http://rpc.eoswatch.info:9000 get account tokencataxxxpermissions: 
     owner     1:    1 EOS8C9tb8QQhZet6WWcYFCWDKHYfjC3W59ugHCD63s7LLDQx6JsNK
        active     1:    1 EOS8C9tb8QQhZet6WWcYFCWDKHYfjC3W59ugHCD63s7LLDQx6JsNK
memory: 
     quota:      1019 KiB    used:     423.4 KiB  

net bandwidth: 
     staked:        100.0000 SYS           (total stake delegated from account to self)
     delegated:       0.0000 SYS           (total staked delegated to account from others)
     used:             14.37 KiB  
     available:        8.626 MiB  
     limit:             8.64 MiB  

cpu bandwidth:
     staked:        100.0000 SYS           (total stake delegated from account to self)
     delegated:       0.0000 SYS           (total staked delegated to account from others)
     used:             12.56 ms   
     available:        1.715 sec  
     limit:            1.728 sec  

SYS balances: 
     liquid:           55.0000 SYS
     staked:          200.0000 SYS
     unstaking:         0.0000 SYS
     total:           255.0000 SYS

producers:     <not voted>

```

```
cleos -u http://rpc.eoswatch.info:9000 push action tokencataxxx setvalue '["tsttoken11", "TESTA", "descr", "blah blah xxx"]' -p tsttoken11

cleos -u http://rpc.eoswatch.info:9000 get account tokencataxxx
permissions: 
     owner     1:    1 EOS8C9tb8QQhZet6WWcYFCWDKHYfjC3W59ugHCD63s7LLDQx6JsNK
        active     1:    1 EOS8C9tb8QQhZet6WWcYFCWDKHYfjC3W59ugHCD63s7LLDQx6JsNK
memory: 
     quota:      1019 KiB    used:     422.5 KiB  

net bandwidth: 
     staked:        100.0000 SYS           (total stake delegated from account to self)
     delegated:       0.0000 SYS           (total staked delegated to account from others)
     used:             14.37 KiB  
     available:        8.626 MiB  
     limit:             8.64 MiB  

cpu bandwidth:
     staked:        100.0000 SYS           (total stake delegated from account to self)
     delegated:       0.0000 SYS           (total staked delegated to account from others)
     used:             12.56 ms   
     available:        1.715 sec  
     limit:            1.728 sec  

SYS balances: 
     liquid:           55.0000 SYS
     staked:          200.0000 SYS
     unstaking:         0.0000 SYS
     total:           255.0000 SYS

producers:     <not voted>

```


```
cleos -u http://rpc.eoswatch.info:9000 push action tokencataxxx setvalues '["tsttoken11", "TESTA", [["title", "Lorem Ipsum"],["org", "Lorem Ipsomania, Inc."], ["url", "http://loremipsumloremipsum.org"], ["email", "lorem@ipsum.org"], ["descr", "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam cursus orci lorem, sit amet volutpat odio sagittis vel. Nulla volutpat sed augue euismod malesuada. Donec mi magna, cursus eu commodo in, malesuada vel augue. Etiam sagittis odio eu lorem mollis, non fringilla purus sodales. Curabitur a semper nulla, nec eleifend risus. Aenean venenatis lorem vulputate dignissim bibendum. Vestibulum bibendum finibus nibh in pellentesque. In porttitor condimentum enim, quis malesuada orci aliquam at. Integer ac est vel enim volutpat bibendum vitae a arcu. Etiam interdum tortor id mauris lobortis, eu mattis lectus gravida. Sed sollicitudin lectus dui, eget suscipit urna bibendum vitae. Sed in commodo lacus. Curabitur turpis felis, bibendum molestie lectus in, vehicula tristique ante. Ut tristique nisl orci, et euismod dolor bibendum ut. Ut imperdiet urna vitae libero pharetra, eget consequat sem porta. Duis pellentesque tempus posuere."]]]' -p tsttoken11

cleos -u http://rpc.eoswatch.info:9000 get account tokencataxxxpermissions: 
     owner     1:    1 EOS8C9tb8QQhZet6WWcYFCWDKHYfjC3W59ugHCD63s7LLDQx6JsNK
        active     1:    1 EOS8C9tb8QQhZet6WWcYFCWDKHYfjC3W59ugHCD63s7LLDQx6JsNK
memory: 
     quota:      1019 KiB    used:     423.4 KiB  

net bandwidth: 
     staked:        100.0000 SYS           (total stake delegated from account to self)
     delegated:       0.0000 SYS           (total staked delegated to account from others)
     used:             14.37 KiB  
     available:        8.626 MiB  
     limit:             8.64 MiB  

cpu bandwidth:
     staked:        100.0000 SYS           (total stake delegated from account to self)
     delegated:       0.0000 SYS           (total staked delegated to account from others)
     used:             12.56 ms   
     available:        1.715 sec  
     limit:            1.728 sec  

SYS balances: 
     liquid:           55.0000 SYS
     staked:          200.0000 SYS
     unstaking:         0.0000 SYS
     total:           255.0000 SYS

producers:     <not voted>

```


```
cleos -u http://rpc.eoswatch.info:9000 push action tokencataxxx setvalues '["tsttoken12", "XTESTXA", [["title", "Lorem Ipsum"],["org", "Lorem Ipsomania, Inc."], ["url", "http://loremipsumloremipsum.org"], ["email", "lorem@ipsum.org"], ["descr", "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam cursus orci lorem, sit amet volutpat odio sagittis vel. Nulla volutpat sed augue euismod malesuada. Donec mi magna, cursus eu commodo in, malesuada vel augue. Etiam sagittis odio eu lorem mollis, non fringilla purus sodales. Curabitur a semper nulla, nec eleifend risus. Aenean venenatis lorem vulputate dignissim bibendum. Vestibulum bibendum finibus nibh in pellentesque. In porttitor condimentum enim, quis malesuada orci aliquam at. Integer ac est vel enim volutpat bibendum vitae a arcu. Etiam interdum tortor id mauris lobortis, eu mattis lectus gravida. Sed sollicitudin lectus dui, eget suscipit urna bibendum vitae. Sed in commodo lacus. Curabitur turpis felis, bibendum molestie lectus in, vehicula tristique ante. Ut tristique nisl orci, et euismod dolor bibendum ut. Ut imperdiet urna vitae libero pharetra, eget consequat sem porta. Duis pellentesque tempus posuere."]]]' -p tsttoken12

cleos -u http://rpc.eoswatch.info:9000 get account tokencataxxxpermissions: 
     owner     1:    1 EOS8C9tb8QQhZet6WWcYFCWDKHYfjC3W59ugHCD63s7LLDQx6JsNK
        active     1:    1 EOS8C9tb8QQhZet6WWcYFCWDKHYfjC3W59ugHCD63s7LLDQx6JsNK
memory: 
     quota:      1019 KiB    used:     424.4 KiB  

net bandwidth: 
     staked:        100.0000 SYS           (total stake delegated from account to self)
     delegated:       0.0000 SYS           (total staked delegated to account from others)
     used:             14.37 KiB  
     available:        8.626 MiB  
     limit:             8.64 MiB  

cpu bandwidth:
     staked:        100.0000 SYS           (total stake delegated from account to self)
     delegated:       0.0000 SYS           (total staked delegated to account from others)
     used:             12.56 ms   
     available:        1.715 sec  
     limit:            1.728 sec  

SYS balances: 
     liquid:           55.0000 SYS
     staked:          200.0000 SYS
     unstaking:         0.0000 SYS
     total:           255.0000 SYS

producers:     <not voted>
```

```
cleos -u http://rpc.eoswatch.info:9000 push action tokencataxxx setvalues '["tsttoken12", "XTESTXB", [["title", "Lorem Ipsum"],["org", "Lorem Ipsomania, Inc."], ["url", "http://loremipsumloremipsum.org"], ["email", "lorem@ipsum.org"], ["descr", "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam cursus orci lorem, sit amet volutpat odio sagittis vel. Nulla volutpat sed augue euismod malesuada. Donec mi magna, cursus eu commodo in, malesuada vel augue. Etiam sagittis odio eu lorem mollis, non fringilla purus sodales. Curabitur a semper nulla, nec eleifend risus. Aenean venenatis lorem vulputate dignissim bibendum. Vestibulum bibendum finibus nibh in pellentesque. In porttitor condimentum enim, quis malesuada orci aliquam at. Integer ac est vel enim volutpat bibendum vitae a arcu. Etiam interdum tortor id mauris lobortis, eu mattis lectus gravida. Sed sollicitudin lectus dui, eget suscipit urna bibendum vitae. Sed in commodo lacus. Curabitur turpis felis, bibendum molestie lectus in, vehicula tristique ante. Ut tristique nisl orci, et euismod dolor bibendum ut. Ut imperdiet urna vitae libero pharetra, eget consequat sem porta. Duis pellentesque tempus posuere."]]]' -p tsttoken12

cleos -u http://rpc.eoswatch.info:9000 get account tokencataxxxpermissions: 
     owner     1:    1 EOS8C9tb8QQhZet6WWcYFCWDKHYfjC3W59ugHCD63s7LLDQx6JsNK
        active     1:    1 EOS8C9tb8QQhZet6WWcYFCWDKHYfjC3W59ugHCD63s7LLDQx6JsNK
memory: 
     quota:      1019 KiB    used:     425.4 KiB  

net bandwidth: 
     staked:        100.0000 SYS           (total stake delegated from account to self)
     delegated:       0.0000 SYS           (total staked delegated to account from others)
     used:             14.37 KiB  
     available:        8.626 MiB  
     limit:             8.64 MiB  

cpu bandwidth:
     staked:        100.0000 SYS           (total stake delegated from account to self)
     delegated:       0.0000 SYS           (total staked delegated to account from others)
     used:             12.56 ms   
     available:        1.715 sec  
     limit:            1.728 sec  

SYS balances: 
     liquid:           55.0000 SYS
     staked:          200.0000 SYS
     unstaking:         0.0000 SYS
     total:           255.0000 SYS

producers:     <not voted>
```

```
cleos -u http://rpc.eoswatch.info:9000 push action tokencataxxx setflag '["tsttoken11", "TESTA", "ready"]' -p tsttoken11


cleos -u http://rpc.eoswatch.^Cfo:9000 push action tokencataxxx setflag '["tsttoken11", "TESTA", "ready"]' -p tsttoken11
cc32d9@ubuntu01:~/build/dappscatalog/contracts$ cleos -u http://rpc.eoswatch.info:9000 get account tokencataxxxpermissions: 
     owner     1:    1 EOS8C9tb8QQhZet6WWcYFCWDKHYfjC3W59ugHCD63s7LLDQx6JsNK
        active     1:    1 EOS8C9tb8QQhZet6WWcYFCWDKHYfjC3W59ugHCD63s7LLDQx6JsNK
memory: 
     quota:      1019 KiB    used:     425.4 KiB  

net bandwidth: 
     staked:        100.0000 SYS           (total stake delegated from account to self)
     delegated:       0.0000 SYS           (total staked delegated to account from others)
     used:             14.37 KiB  
     available:        8.626 MiB  
     limit:             8.64 MiB  

cpu bandwidth:
     staked:        100.0000 SYS           (total stake delegated from account to self)
     delegated:       0.0000 SYS           (total staked delegated to account from others)
     used:             12.56 ms   
     available:        1.715 sec  
     limit:            1.728 sec  

SYS balances: 
     liquid:           55.0000 SYS
     staked:          200.0000 SYS
     unstaking:         0.0000 SYS
     total:           255.0000 SYS

producers:     <not voted>
```


```
cleos -u http://rpc.eoswatch.info:9000 push action tokencataxxx settags '["tsttoken11", "TESTA", ["game","oracle","tentacle","bdsm","kraken","awesome","fry","laurie","psycho","wiki"]]' -p tsttoken11

cleos -u http://rpc.eoswatch.info:9000 get account tokencataxxxpermissions: 
     owner     1:    1 EOS8C9tb8QQhZet6WWcYFCWDKHYfjC3W59ugHCD63s7LLDQx6JsNK
        active     1:    1 EOS8C9tb8QQhZet6WWcYFCWDKHYfjC3W59ugHCD63s7LLDQx6JsNK
memory: 
     quota:      1019 KiB    used:     429.4 KiB  

net bandwidth: 
     staked:        100.0000 SYS           (total stake delegated from account to self)
     delegated:       0.0000 SYS           (total staked delegated to account from others)
     used:             14.37 KiB  
     available:        8.626 MiB  
     limit:             8.64 MiB  

cpu bandwidth:
     staked:        100.0000 SYS           (total stake delegated from account to self)
     delegated:       0.0000 SYS           (total staked delegated to account from others)
     used:             12.56 ms   
     available:        1.715 sec  
     limit:            1.728 sec  

SYS balances: 
     liquid:           55.0000 SYS
     staked:          200.0000 SYS
     unstaking:         0.0000 SYS
     total:           255.0000 SYS

producers:     <not voted>
```

```
cleos -u http://rpc.eoswatch.info:9000 push action tokencataxxx settags '["tsttoken12", "XTESTXA", ["game","oracle","tentacle","bdsm","kraken","awesome","fry","laurie","psycho","wiki"]]' -p tsttoken12

cleos -u http://rpc.eoswatch.info:9000 get account tokencataxxxpermissions: 
     owner     1:    1 EOS8C9tb8QQhZet6WWcYFCWDKHYfjC3W59ugHCD63s7LLDQx6JsNK
        active     1:    1 EOS8C9tb8QQhZet6WWcYFCWDKHYfjC3W59ugHCD63s7LLDQx6JsNK
memory: 
     quota:      1019 KiB    used:     433.3 KiB  

net bandwidth: 
     staked:        100.0000 SYS           (total stake delegated from account to self)
     delegated:       0.0000 SYS           (total staked delegated to account from others)
     used:             14.37 KiB  
     available:        8.626 MiB  
     limit:             8.64 MiB  

cpu bandwidth:
     staked:        100.0000 SYS           (total stake delegated from account to self)
     delegated:       0.0000 SYS           (total staked delegated to account from others)
     used:             12.56 ms   
     available:        1.715 sec  
     limit:            1.728 sec  

SYS balances: 
     liquid:           55.0000 SYS
     staked:          200.0000 SYS
     unstaking:         0.0000 SYS
     total:           255.0000 SYS

producers:     <not voted>
```

```
cleos -u http://rpc.eoswatch.info:9000 push action tokencataxxx settags '["tsttoken12", "XTESTXB", ["sagittis", "odio", "eu", "lorem", "mollis", "non", "fringilla", "purus", "sodales", "commodo"]]' -p tsttoken12

cleos -u http://rpc.eoswatch.info:9000 get account tokencataxxxpermissions: 
     owner     1:    1 EOS8C9tb8QQhZet6WWcYFCWDKHYfjC3W59ugHCD63s7LLDQx6JsNK
        active     1:    1 EOS8C9tb8QQhZet6WWcYFCWDKHYfjC3W59ugHCD63s7LLDQx6JsNK
memory: 
     quota:      1019 KiB    used:     437.1 KiB  

net bandwidth: 
     staked:        100.0000 SYS           (total stake delegated from account to self)
     delegated:       0.0000 SYS           (total staked delegated to account from others)
     used:             14.37 KiB  
     available:        8.626 MiB  
     limit:             8.64 MiB  

cpu bandwidth:
     staked:        100.0000 SYS           (total stake delegated from account to self)
     delegated:       0.0000 SYS           (total staked delegated to account from others)
     used:             12.56 ms   
     available:        1.715 sec  
     limit:            1.728 sec  

SYS balances: 
     liquid:           55.0000 SYS
     staked:          200.0000 SYS
     unstaking:         0.0000 SYS
     total:           255.0000 SYS

producers:     <not voted>
```

```
cleos -u http://rpc.eoswatch.info:9000 set account permission tokencataxxx active '{"threshold": 1, "keys":[{"key":"EOS8C9tb8QQhZet6WWcYFCWDKHYfjC3W59ugHCD63s7LLDQx6JsNK", "weight":1}] , "accounts":[{"permission":{"actor":"tokencataxxx","permission":"eosio.code"},"weight":1}], "waits":[] }' owner -p tokencataxxx
```