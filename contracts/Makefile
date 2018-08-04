CONTRACT=dappscatalog

all: $(CONTRACT).abi $(CONTRACT).wast

$(CONTRACT).abi: catalog.hpp *_rc.md

$(CONTRACT).wast: catalog.hpp



%.wast: %.cpp
	eosiocpp -o $@ $<

%.abi: %.cpp
	eosiocpp -g $@ $<

clean:
	rm -f $(CONTRACT).abi $(CONTRACT).wast $(CONTRACT).wasm
