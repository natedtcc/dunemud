// quest_obj.c - Juris' Living Sietch Quest Object - Bebop 2021

inherit "/obj/quest_obj";

void reset(int arg)
{
    ::reset(arg);
    if(arg) return;
    
    set_name("stolen_knife");
    set_hint(
    "A powerful Fremen tribe is in an uproar. Their sietch was recently\n"
    +"the victim of a brutal Harkonnen military campaign, which resulted\n"
    +"in the theft of a sacred crysknife. A Wise Fremen seeks an audience\n"
    +"with anyone who is able to procure this plundered treasure; Seek him\n"
    +"out! He can be found in a Fremen Sietch, south-east of the Arrakeen\n"
    +"Astro Port, in the Arrakis Desert.\n");

    set_points(3);
    set_listname("The Lost Crysknife");

}
