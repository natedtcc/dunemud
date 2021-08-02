// quest_obj.c - Fate's Hospital Quest Object - Bebop 2021

inherit "/obj/quest_obj";

void reset(int arg)
{
    ::reset(arg);
    if(arg) return;
    
    set_name("tainted_spice");
    set_hint(
      "There's a large, angry chef at the Tleilax Hospital who's been\n"
      +"getting bad batches of spice lately. You hear that the residents\n"
      +"of the Hospital have been getting even sicker, and the tainted\n"
      +"spice is likely to blame. The chef will reward anyone who can\n"
      +"provide him with a clean product. Beware - this angry chef will\n"
      +"not tolerate anything but the highest quality. Can you find the\n"
      +"pure, liquid spice hidden away deep within the Hospital grounds?\n");

    set_points(3);
    set_listname("Tainted Spice");

}
