
inherit "/obj/player/monster";
int count;
void reset(int arg)
{
  ::reset(arg);
  if (!arg) {

    set_name("junkie");
    set_alias( ({"junkie", "addict"}) );

    set_short("$BOLDA $REDSapho $YELLOWJunkie$OFF");
    set_long(
      "This man looks to be in a state of catatonic despair, most likely\n"
      +"from the sapho. You look into the junkie's eyes and see they are\n"
    +"almost devoid of life. He is currently sprawled out on one of the\n"
    +"bean-bags in the room, mouth agape, and pipe still in hand. It's\n"
    +"smoking a tiny bit, hopefully he doesn't drop the damn thing and\n"
      +"start a fire!");

    set_gender(1);
    set_race("fremen");
    set_level(5);
    set_property("level_override");
    set_non_combat_chats (12, ({
      ({ "The Junkie groans loudly.", "echo" }),
      ({ "The Junkie drools on himself a bit.","echo" }),
      ({ "You hear a tapping sound as the Junkie empties his pipe.","echo" }),
      ({ "Junkie takes a huge puff off of his sapho pipe!", "echo"}),
      ({ "Junkie asks: Got any sapho?","echo" }) 
      }));

    set_combat_chats (20, ({
      ({ "The junkie slobbers all over you!!", "echo" }),
      ({ "The junkie scratches you with his disgusting fingernails!.", "echo" })
      }));
  }
}
