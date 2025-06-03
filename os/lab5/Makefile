head=vmrp.h
srcs=vmrp.cc
objs=$(BINDIR)/vmrp.o
opts=-w -g -c
BINDIR=bin

all: $(BINDIR)/vmrp

$(BINDIR)/vmrp: $(objs) | $(BINDIR)
	g++ $(objs) -o $(BINDIR)/vmrp

$(BINDIR)/vmrp.o: $(srcs) $(head) | $(BINDIR)
	g++ $(opts) $(srcs) -o $(BINDIR)/vmrp.o

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -f $(BINDIR)/vmrp $(BINDIR)/*.o
	rmdir $(BINDIR) 2>/dev/null || true