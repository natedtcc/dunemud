// quest_obj.c - Sapho Area Quest Object - Bebop 2021

inherit "/obj/quest_obj";

void reset(int arg)
{
    if(arg) return;
    
    // Maybe name = id? 
    set_name("sapho_facility");
    set_hint(
      "Odd happenings have been reported on Salusa. Whispers tell of a place where\n"
      +"mentats and normal folk alike congregate to enjoy a new refined version of\n"
      +"the popular drink, Sapho. According to local lore, the facility that creates\n"
      +"this extremely addicting substance is located somewhere within an area that\n"
      +"caters to their customers. What's more, there seems to be someone on the\n"
      +"inside, attempting to gain valuable insight on this popular new drug...\n");

    set_points(2);
    set_listname("The Sapho Den");
}
