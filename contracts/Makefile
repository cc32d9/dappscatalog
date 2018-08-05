CONTRACT=dappscatalog

all: $(CONTRACT).wast $(CONTRACT).abi

$(CONTRACT).abi: catalog.hpp *_rc.md

$(CONTRACT).wast: catalog.hpp



%.wast: %.cpp
	eosiocpp -o $@ $<

%.abi: %.cpp
	perl mkabi.pl $<

clean:
	rm -f $(CONTRACT).abi $(CONTRACT).wast $(CONTRACT).wasm
