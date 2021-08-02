// Added Sapho Den entrance / ldesc -  Bebop 2021

inherit "room/newroom";

#include "/d/Imperial/domain/include/defines.h"

mapping commonitems;

reset(arg) {
  ::reset(arg);
  if(arg)return;

    set_outdoor();
    set_light(1);

    short_desc = "Eastern Road";
    long_desc =
"You are at a corner of the eastern road, you can see rows of low buildings\n"+
"on each side of it. The road is dirty, worn out and it makes a 90 turn here.\n"+
"You can see the main road when looking west, while the rest of the Eastern\n"+
"Road, with a pub and a shop, is on the northern side.\n"+
"There is a pleasant aroma emanating from the east.\n";

    long_night_desc =
"You are at a corner of the eastern road. Five streetlights stand along the\n"+
"the road, only one works so it's pretty dark here. Behind those lights, rows\n"+
"of low buildings prevent any view from what's behind them. You can head\n"+
"north towards a pub and a shop or west towards the main road.\n"+
"There is a pleasant aroma emanating from the east.\n";

    dest_dir = ([
      "west":"/d/Imperial/domain/east1",
      "north":"/d/Imperial/domain/2main2",
      "east":"/d/Imperial/bebop/sapho/room/dhall1.c" ]);

    nightitems = ([
     "lights":"Four of them are broken, the other one tries but fails\n"+
              "to completely illuminate the corner",
     "streetlights":"=lights"
            ]);
    commonitems = ([
     "buildings":"They look like they've been here for a long time,\n"+
                 "although they were probably build yesterday.",
     "low buildings":"=buildings",
     "rows of low buildings":"=buildings",
     "corner":"Just look around you",
     "road":"The Eastern Road",
     "eastern road":"This is the Eastern Road, it has several rows"
             +" of low buildings on each side.",
     "main road":"The main road, it runs along the astroport",
     "astroport":"You can't see it from here",
     "pub":"You should go and check it out",
     "shop":"=pub"
             ]);

    if(day()) items=commonitems;
    else items=commonitems+nightitems;
    set_planet("salusa");
}

notify_time_event(int day_stat)
{
  switch(day_stat)
  {
    case 1:
    case 2: items = commonitems;
            break;
    default: items = commonitems + nightitems;
             break;
  }
}

init()
{
 ::init();
 this_player()->add_explorer_flag(object_name(TO));
}
