WXDIR = d:/wxCyg

top_srcdir = $(WXDIR)
top_builddir = $(WXDIR)/build

PROGRAM=gui
GAME=tarmellion

APPEXTRADEFS =  -I. -I..

RULESET_OBJECTS = extra.o monsters.o rules.o world.o

ENGINE_OBJECTS = alist.o aregion.o army.o astring.o battle.o faction.o \
  fileio.o game.o gamedata.o gamedefs.o gameio.o items.o \
  market.o modify.o monthorders.o npc.o object.o orders.o parseorders.o \
  production.o runorders.o shields.o skills.o skillshows.o specials.o \
  spells.o template.o unit.o

GUI_OBJECTS = $(PROGRAM).o alistex.o config.o edit.o editaux.o map.o tree.o

OBJECTS = $(GUI_OBJECTS) \
          $(patsubst %.o, ../$(GAME)/%.o, $(RULESET_OBJECTS)) \
          $(patsubst %.o, ../%.o, $(ENGINE_OBJECTS)) \
          ../i_rand.o

include $(WXDIR)/build/src/makeprog.env