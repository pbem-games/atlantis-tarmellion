WXDIR = E:/wxCyg

top_srcdir = $(WXDIR)
top_builddir = $(WXDIR)/build

PROGRAM=gui
GAME=tarmellion

APPEXTRADEFS =  -g -I. -I.. -Wall -Wno-deprecated

RULESET_OBJECTS = extra.o monsters.o rules.o world.o

ENGINE_OBJECTS = alist.o aregion.o army.o astring.o battle.o faction.o \
  fileio.o game.o gamedata.o gamedefs.o gameio.o items.o \
  market.o modify.o monthorders.o npc.o object.o orders.o parseorders.o \
  production.o runorders.o shields.o skills.o skillshows.o specials.o \
  spells.o template.o unit.o

GUI_OBJECTS = $(PROGRAM).o aextend.o config.o edit.o editaux.o list.o map.o \
  options.o run.o tree.o

OBJECTS = $(GUI_OBJECTS) \
          $(patsubst %.o, ../$(GAME)/%.o, $(RULESET_OBJECTS)) \
          $(patsubst %.o, ../%.o, $(ENGINE_OBJECTS)) \
          ../i_rand.o

include $(WXDIR)/build/src/makeprog.env