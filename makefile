CC			= gcc
CXX			= g++
CFLAGS		= -Wall -std=c99 -g -O0
CXXFLAGS	=
CPPFLAGS	= 
LDFLAGS 	=  
LIBS		= 

TARGET		= $(notdir $(shell pwd))
SRCDIR		= ./src
OBJDIR		= ./build
INCDIR		= ./include

SUFFIXES	= .c .cpp .asm

SRCS		= $(foreach suffix, $(SUFFIXES), $(wildcard $(SRCDIR)/*$(suffix)))
OBJS		= $(addsuffix .o,$(addprefix $(OBJDIR)/, $(notdir $(basename $(SRCS) ) ) ) )

INCOPT		= $(addprefix -I,$(INCDIR) )

#$(warning $(SRCS))

all:$(TARGET)

$(TARGET):$(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)


$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(warning $@)
	$(CC) $(CFLAGS) $(INCOPT) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(warning $@)
	$(CC) $(INCOPT) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.asm
	@mkdir -p $(OBJDIR)
	$(warning $@)
	$(CC) $(INCOPT) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)

test:$(TARGET)
	test/test.sh

.PHONY: all clean