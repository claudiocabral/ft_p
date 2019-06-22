NAME := server

SRCS := \
	srcs/main.c \
	srcs/server.c

CFLAGS =\
	-Wextra -Werror -Wall -g

#$(if $(DEBUG), CFLAGS += -g, )

OBJS := $(SRCS:srcs/%.c=objs/%.o)

DEPS := $(OBJS:objs/%.o=.deps/%.d)

INCLUDES := \
	-Iincludes

all: $(NAME)

$(NAME): $(OBJS)
	cc $(CFLAGS) $(OBJS) -o $@

objs/%.o: srcs/%.c | $(dir objs/%.o) $(dir .deps/%.d)
	cc -c $(CFLAGS) $(INCLUDES) $< -MMD -MP -MT $@ -MF $(@:objs/%.o=.deps/%.d) -o $@

.deps/%.d: ;

%/:
	mkdir -p $@

include $(DEPS)

clean:
	$(call remove_if_exists, $(OBJS) $(DEPS) objs .deps)

fclean: clean
	$(call remove_if_exists, $(NAME))

re:
	@$(MAKE) --no-print-directory fclean
	@$(MAKE) --no-print-directory all

.PHONY: all
.PRECIOUS: %/

define remove_if_exists
$(eval LIST:=)
$(foreach elem, $(1), $(if $(shell [ -e $(elem) ] && echo 1),$(eval LIST += $(elem)),))
$(if $(LIST), @rm -rfv $(LIST))
endef
