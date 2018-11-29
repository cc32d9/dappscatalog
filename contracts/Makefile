CONTRACT=dappscatalog

all: $(CONTRACT).wasm $(CONTRACT).abi

$(CONTRACT).abi: $(CONTRACT).clauses.md $(CONTRACT).contracts.md

%.wasm: %.cpp
	eosio-cpp -o $@ $<

%.abi: %.cpp
	eosio-abigen -contract=$(CONTRACT) -output=$@ $<

clean:
	rm -f $(CONTRACT).wasm $(CONTRACT).abi
