/* Tech's Quaters */
/* by Kiza 12-Jul-1996 */
/* cleaned up / upgraded by Bebop July 2018 */

inherit "/room/newroom";
inherit "/d/Fremen/bebop/sietch/include/area_utils.c";

void reset(int arg){
  ::reset(arg);
  place_objects("/d/Atreid/kiza/ubase/npc/technician.c", 2);
  if(arg) return;
   
  set_light(0);
  set_short("Technician Sleeping Quarters");
  set_long(
  "A small room roughly 12' X 8', with clean white walls, and the only \n"+
  "furniture being a desk, bed and shelving.  It is small and functional\n"+
  "but also has that warm lived in feeling about it.\n\n"+
  "The door to the east leads back to the Residential Hallway.\n\n");

  add_item("desk",
  "A finely crafted wooden desk, which has paperwork stacked in neat piles.\n"+
  "It looks as though the owner has been hard at work on it.");
  add_item("bed",
  "A rather comfortable looking bed which you think you better not try using,\n"+
  "the owner would not be pleased.");
  add_item("shelves",
  "There are many shelves with items on located around the room.\n");
  add_item("shelf",
  "One of many shelves located around the room, stacked with many items\n");
  add_item("items",
  "There are many items stacked upon the shelf, books and their knick-knacks\n"+
  "although nothing overly interesting.");
  add_item("shelving","=shelves");
  add_item("walls",
  "These walls look clean and pristine, it is obvious that the owner of this\n"+
  "room prides cleanliness.");
  
  add_exit("east","/d/Atreid/kiza/ubase/res3.c");
}

init() {
::init();

  add_action("search","search");
}


search(arg){

  if(arg == "bed") {
    write("You find nothing special in this bed.\n");
  }
  else {
    write("Search what??.\n");
  }
  return 1;
}

