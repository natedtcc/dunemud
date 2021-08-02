// Bebop's Lounge - Bebop 2018

#include <mudlib.h>
inherit ROOM;

void reset(int arg)
{

  ::reset(arg);
  if(arg) return;

  set_light(1);
  set_short(" Portal Room ");

  set_long(
      "\nJust a bunch of portals here.\n");

  add_exit("wal", "/d/Space/stryder/planets/wall_station");
  add_exit("arr", "/d/Space/stryder/planets/arr_station");
  add_exit("cal", "/d/Space/stryder/planets/cal_station");
  add_exit("sal", "/d/Space/stryder/planets/sal_station");
  add_exit("tle", "/d/Space/stryder/planets/tle_station");
  add_exit("chap", "/d/Space/stryder/planets/chap_station");
  add_exit("gp", "/d/Space/stryder/planets/gp_station");
  add_exit("ix", "/d/Space/stryder/planets/ix_station");

}