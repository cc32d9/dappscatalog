CONTRACT=dappscatalog

all: $(CONTRACT).wasm

$(CONTRACT).abi: catalog.hpp *_rc.md

%.wasm: %.cpp
	eosio-cpp -o $@ $<

%.abi: %.cpp
	eosio-abigen --contract=$(CONTRACT) --output=$@ $<

clean:
	rm -f $(CONTRACT).wasm
