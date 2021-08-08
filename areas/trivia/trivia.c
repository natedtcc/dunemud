// trivia.c
// based on deathknight's original trivia npc
// played a little differently now though, the question set is huge
// and the questions are randomly picked from the list, and the first
// person to MAX_POINTS points is the winner.
// Sauron
// Old questions file -> "/open/sauron/questions"

// Updated to use the OpenTBD API (https://opentdb.com)
// Players can now start their own games (3 or more required)

// Trivia is a lot different now. Many many changes have been made,
// but much of Sauron's logic remains intact. I'll let the reader
// decide if that's good or not.
// Bebop 08/1/2021

inherit "/obj/player/monster";

#define API        "/d/Atreid/domain/trivia/question_handler"
#define HEARER     "/players/arfang/listenner"  // Hearer
#define MAX_POINTS     200             // How many points do we play for?
#define MIN_POINTS     200             // Point threshold to be reached by all
#define POINT_AWARD    10              // How many points for a right answer?
#define WARMUP_TIME    25               // Time for players to join before start
#define ANSWER_TIME    15              // How long will players be able to answer?
#define BREAK_TIME     15              // Time between rounds 
#define OT_WARMUP_TIME 5               // Warmup for sudden death
#define MAX_ROUNDS     50              // Max number of rounds per game
#define DEBUG          0
#define DEBUG_TARGET "bebop"

mapping players,        // ([ name: score; correct guesses, last result ])
        new_questions,  // ([<n>: category; question; letter_answer, full_answer, all_answers])
        answered;       // Who has answered, and did they get it right?

string  *old_questions,   // Questions we've already asked
        *leader,          // Current game leader
        category,         // Category of the question
        curr_question,    // Question we're on right now
        letter_answer,    // The letter answer to the current question
        full_answer,      // The full correct answer
        *all_answers;     // All answers, wrong or right

object  GAMEMASTER;       // If a wiz is present, make him GM

int     round,            // What round are we in?
        automated=1,      // Automated rounds (enabled)
        total_points=0,   // Total points of all players
        number_checked,   // How man answers we've checked
        number_guessed,   // How many answers yet to check
        in_round,         // Are we currently in a round?
        is_overtime,      // Are we in Sudden Death?
        game_over;        // Is the game over?

// Function Prototyping
int add_player(string who);      // Add a player to the game
int remove_player(string who);   // Remove a player from the game
int do_help(string who);         // Give a player help
int control(string str);         // Control commands for the GM
int do_round();                  // Begin a trivia round
int player_check();              // Check if there's enough players to play
int award_points();              // Give points to the winners of the round
int check_points();              // Check to see if the game is over
int end_game(string *winners);   // End the game, congratulate winners
int do_answer(string who, string what);     // Check an answer
int got_right(string f);         // Got it right!
int got_wrong(string f);         // Oot it wrong :(
int do_add(string arg);          // Add points to someone's score
int do_subtract(string arg);     // Subtract points from someone's score
int quit_trivia();               // End the game suddenly
int is_automated();              // Is the game automated?
int register();                  // Register with the tell listener
int do_score();                  // Show the game score
int score_overtime();            // Check score of Sudden Death rounds
int set_questions();             // Set the question mapping to reflect API data
int control_trivia(string who);   // Give full control to a GM

void do_question();              // (GM command) Show the question, and the time left to answer
void setup_question();           // Grab a question and parse it into question and answer
void display_question();         // (Game generated) Show the question, and the time you have to answer
void startup();                  // Initialize trivia, and set everything up
void reset_last_result();        // Reset all players' last result (if they answered right or not)
void end_round();                // End a current round
void destroy_game();             // Destroy this object + the API object
void set_automated(int state);   // Enable automation
void start_round_automation();   // Start the round automation sequence
void start_game_automation();    // Start the game automation sequence
void update_api();               // Update the API data


void reset(int arg)
{
  ::reset(arg);
  if (!arg) {
    string short = "$BOLD$REDTriviaBot$OFF v2 ";
    switch(random(6)){
      case 0: short+="(nasty)"; break;
      case 1: short+="(wicked)"; break;
      case 2: short+="(good)"; break;
      case 3: short+="(pure)"; break;
      case 4: short+="(evil)"; break;
      case 5: short+="(holy)"; break;
    }
    set_level(100);
    set_prop("no_fight", 1);
    set_prop("no_voice",1);
    set_name("TriviaBot");
    set_alias(({"trivia_npc", "triviabot", "trivia"}));
    set_short(short);
    set_long("\
This is a large, shiny robotic entity. He handles all the\n\
complexities of Trivia with ease! If you need help, type\n\
'tell bot /help' or 'help trivia' for more information.\n");

    enable_commands();
    players = ([ ]);
    answered = ([ ]);
    // waiting = ({ });
    new_questions = ([]);
    old_questions = ({ });
  }
    register();
}

// Start the trivia game, let everyone know we're here
void startup()
{
  // Pulled this in favor of shout - maybe ...
  //HEARER->listen("A new game of Trivia is beginning!\n");
  round = 1;
  players = ([ ]);
  old_questions = ({ });

  start_game_automation();
}

// Give control to the GM
init()
{
  ::init();

  if (DEBUG){
    add_action("full_control", "trivia");
  }

  else add_action("control","trivia");

}

int do_answer(string who, string what)
{
  object player;

  string temp_letter,
         temp_full;

  player = find_player(who);
  if(!answered) answered = ([ ]);

  if(!player)
    return 1;

  if(!present(player, environment(this_object())))
  {
    tell_object(player,
      "TriviaBot tells you: Whats that? Come closer...$OFF\n");
    return 1;
  }

  // added by Seeker 20010122
  if(!member(players, who))
  {
    tell_object(player,
      "TriviaBot tells you: You have to join first. Type 'trvia join' to join!\n");
    return 1;
  }

  if(!in_round)
  {
    tell_object(player,
      "TriviaBot tells you: We are not in a round right now!\n");
    return 1;
  }

  if(member(answered, who))
  {
    tell_object(player,
      "TriviaBot tells you: You've already answered this question!\n");
    return 1;
  }

  what = lower_case(what);
  temp_letter = lower_case(letter_answer);
  temp_full = lower_case(full_answer);

  if(what == temp_letter || what == temp_full)
  {
    got_right(who);
    return 1;
  }
  else
  {
    got_wrong(who);
    return 1;
  }
}


int add_player(string who)
{
  object player = find_player(who);

  if (is_overtime){
    return notify_fail("Sorry, you cannot join during Sudden Death!\n");
  }

  if(!players) { players = ([ ]); }

  if(!player)
  {
    write("Could not find target.\n");
    return 1;
  }

  if(game_over)
  {
    tell_object(player, "TriviaBot tells you: The game is already over.\n");
    return 1;
  }

  if(member(players, who))
  {
    tell_object(player, "TriviaBot tells you: You are already playing!\n");
    return 1;
  }

  players += ([ who: 0; 0; 0,]);
  tell_room(environment(this_object()), parse_ansi(
    "TriviaBot announces: $CYAN"+capitalize(player->query_name())
    +"$OFF has joined Trivia!\n"));
  tell_object(player, 
  "TriviaBot tells you: type 'help trivia' for more information.$OFF\n");
  return 1;
}

int remove_player(string who)
{
  object player = find_player(who);

  if(!players){
    players = ([ ]);
    write("TriviaBot tells you: You weren't playing!$OFF\n");
    return 1;
  }

  if(!member(players, who))
  {
    write("TriviaBot tells you: You weren't playing!\n");
    return 1;
  }

  if(!player)
  {
    write("TriviaBot mutters: Not a player.\n");
    return 1;
  }

  if(game_over)
  {
    tell_object(player, "TriviaBot tells you: The game is already over.\n");
    return 1;
  }

  m_delete(players, who);
  tell_object(player, "TriviaBot tells you: Ok, goodbye!\n");
  tell_room(environment(this_object()), parse_ansi(
    "TriviaBot announces: $RED"+capitalize(player->query_name())
    +"$WHITE has quit Trivia!\n"));
}

int do_help(string who)
{
  object player;

  player = find_player(who);
  if(!player)
  {
    write("TriviaBot mutters: Not a player.\n");
    return 1;
  }

  tell_object(player, 
              "TriviaBot tells you: Hey, I'm TriviaBot. I'll present you with 50 questions.\n"+
              "TriviaBot tells you: If you get one right, you get $BOLD$GREEN"+POINT_AWARD+"$OFF points.\n"+
              "TriviaBot tells you: Points are not $BOLD$REDdeducted$OFF for wrong answers.\n"+
              "TriviaBot tells you: The first player to get $BOLD$CYAN"+MAX_POINTS+"$OFF points is the winner!\n"+
              "TriviaBot tells you: You can 'tell' me your answers. Questions are in multiple choice format.\n"+
              "TriviaBot tells you: For example: 'tell triviabot b', if you think B is the right answer.\n"+
              "TriviaBot tells you: Good luck! See 'help trivia for information.\n");
  return 1;
}

// Trivia controls

int control(string str)
{
  if(!str){
    return notify_fail(
      "\nTrivia Commands:\n\n"
      +"'trivia join'  - Join the current game of Trivia.\n"
      +"'trivia quit'  - Leave the current game of Trivia.\n"
      +"'trivia score' - Check the scoreboard.\n\n");  }  

  string player = lower_case(this_player()->query_name());

  int double;

  switch(str)
  {
    case "score": do_score(); break;
    case "join": add_player(player); break;
    case "quit": remove_player(player); break;
    case "help": do_help(player); break;
    default:
      write("TriviaBot tells you: Command not available.\n");
      break;
  }
  return 1;
}

// Do a round of trivia!
int do_round()
{
  if (!players){
    return notify_fail("$BOLDCannot start a round without players! Aborting!$OFF\n");
    destroy_game();
    }

  reset_last_result();
  int max_rec;
  string* names;

  max_rec = 50;

  if(!old_questions)
    old_questions = ({ });

  if(in_round)
  {
    write("TriviaBot tells you: We are already in a round!\n");
    return 1;
  }

  if(game_over)
  {
    write("TriviaBot tells you: The game is already over.\n");
    return 1;
  }

  in_round = 1;

  setup_question();

  // added by Seeker
  // removed by Bebop
  // waiting = ({});

  number_checked = number_guessed = 0;

  display_question();
  call_out("end_round", ANSWER_TIME);
  return 1;
}

void setup_question()
{
  all_answers = ({});
  category = new_questions[round-1, 0];
  curr_question = new_questions[round-1, 1];
  letter_answer = new_questions[round-1, 2];
  full_answer = new_questions[round-1, 3];
  all_answers = new_questions[round-1, 4];
}

void display_question()
{
  string output;
  if (is_overtime) 
    say(parse_ansi("\n$BOLD$REDS U D D E N  D E A T H !$OFF\n"));
  

  else{ 
  say(parse_ansi(
    "\n$UNDERL$BOLDQuestion for round:$OFF$BOLD$YELLOW "
    +round+"/"+MAX_ROUNDS+"$OFF$BOLD\n"));
  }

  output = parse_ansi(sprintf("$BOLD%'-'-79s\n\n", "-"));
  output += parse_ansi(sprintf(
    "$BOLD$UNDERLCategory:$OFF$BOLD$BLUE  "+category
    +"\n\n$WHITE$UNDERLThe question is:$OFF"
    +"$BOLD$GREEN\n\n%s$WHITE\n\n\n",curr_question));
  output += parse_ansi(sprintf("$UNDERLPossible answers:$OFF$BOLD\n\n"));

  // Show the answers
  if (sizeof(all_answers)==2){
    output += parse_ansi(
      "$WHITEA) $CYAN" + all_answers[0] 
      + "  $WHITEor  B)$RED "+ all_answers[1]+"$WHITE\n\n");
  }
  
  else{
    for (int i=0 ;i<sizeof(all_answers);i++){
      switch(i) {
        case 0 : output += parse_ansi("$WHITEA) $CYAN" + all_answers[i]+"\n"); break;
        case 1 : output += parse_ansi("$WHITEB) $CYAN" + all_answers[i]+"\n"); break;
        case 2 : output += parse_ansi("$WHITEC) $CYAN" + all_answers[i]+"\n"); break;
        case 3 : output += parse_ansi("$WHITED) $CYAN" + all_answers[i]+"\n\n"); break;
      }
    }
  }

  output += parse_ansi(sprintf(
    "$BOLD$WHITEYou have $GREEN%d$WHITE seconds to answer.\n", ANSWER_TIME));
  output += parse_ansi(sprintf("%'-'-79s$OFF\n", "-"));

  say(output);
}


// This shit is really hard to read, so I (and Sauron) apologize.
// Display the score
int do_score()
{
  // Format testing
  // players += (["Spips": 100; 0; 0,]);
  // players += (["Corona": 20; 0; 0,]);
  // players += (["Abbazabba": 30; 0; 0,]);
  // players += (["Giraffe": 40; 0; 0,]);
  // players += (["Jabroni": 0; 0; 0,]);
  // players += (["Carlos": 50; 0; 0,]);
  // players += (["Ronald": 0; 0; 0,]);
  // players += (["Bepp": 140; 0; 0,]);

  if (sizeof(players)<1){
    return notify_fail("There are no players to show on the scoreboard!\n");
  }
  string *names,
         *left,
         *right,
         output;

  mapping top,
          bottom;

  int count,
      howmany,
      i;

  names = m_indices(players);
  output = "";

  top = ([ ]);
  bottom = ([ ]);

  output += sprintf("\n$BOLDTrivia Scores for round $CYAN%d$WHITE\n%'-'60s", round, "-");
  output += sprintf("$BOLD$OFF\n", "-");
  write(parse_ansi(output));
  output = "";

  left = ({ });
  right = ({ });

  if(sizeof(names) > 1)
  {
    names = sort_array(names, "by_high_score", this_object());
    left = names[0..sizeof(names)/2];
    for(i=0;i<sizeof(names);i++)
    {
      if(member(left, names[i]) == -1)
        right += ({ capitalize(names[i]) });
    }

    for(i=0; i < sizeof(names)/2+1; i++)
    {
      if(sizeof(left) >= i)
      {
        top += ([ left[i]: players[left[i], 0]; players[left[i], 1], ]);
      }
      if(sizeof(right)-1 >= i)
      {
        bottom += ([ right[i]: players[right[i], 0]; players[right[i], 1], ]);
      }
    }
  }
  else
  {
    write(parse_ansi(sprintf(
      "$BOLD%-3s$CYAN%-15s$GREEN%-4d$WHITE(%d%%) \n",
      "1.", capitalize(names[0]), players[names[0], 0],
      players[names[0], 1]*100/(round > 0 ? round : 1))));

     write(parse_ansi(sprintf("$BOLD%'-'60s$OFF\n\n", "-")));
     return 1;
  }

  howmany = sizeof(m_indices(top));
  left = m_indices(top);
  left = sort_array(left, "by_high_score");
  right = m_indices(bottom);
  right = sort_array(right, "by_high_score");

  // Scores are printed like this:
  // 1. Bebop          0   (0%)

  for(i=0; i<howmany; i++)
  {
      output = "$BOLD("+to_string(top[left[i], 1]*100/(round > 1 ? round : 1))+"\%)";
      
      // Highlight the leader
      if (i==0 && players[names[i], 0] != 0){
        write(parse_ansi(sprintf(
        "$BOLD%-3s$RED%-15s$OFF$BOLD$YELLOW%-4d$WHITE%-8s", 
        to_string(i+1)+".", capitalize(left[i]), top[left[i], 0], output)));
      }

      else write(parse_ansi(sprintf(
        "$BOLD%-3s$CYAN%-15s$GREEN%-4d$WHITE%-8s", 
        to_string(i+1)+".", capitalize(left[i]), top[left[i], 0], output)));

      if(sizeof(right) > i)
      {
        output = "("+to_string(top[right[i], 1]*100/(round > 1 ? round : 1))+"\%)";
        write(parse_ansi(sprintf(" $BOLD%-3s$CYAN%-15s$GREEN%-4d$WHITE%-8s", 
          to_string(sizeof(left)+1+i)+".",capitalize(right[i]), bottom[right[i], 0],
            "("+to_string(bottom[right[i], 1]*100/(round > 1 ? round : 1))+"%)")));
      }
      write("\n");
  }
  write(parse_ansi(sprintf("$BOLD%'-'60s$OFF\n\n", "-")));

  return 1;
}

status by_high_score(string s, string f)
{
  return players[s, 0] < players[f, 0];
}

int got_right(string who)
{
  say(parse_ansi(
    "$GREENTriviaBot says: "+capitalize(who)+" got it right!$OFF\n"));
  players[who, 1]++;
  answered += ([ who: 1, ]);
  players[who, 2] = 1;
  return 1;
}

int got_wrong(string who)
{
  say(parse_ansi(
    "$CYANTriviaBot says: "+capitalize(who)+" got it wrong!$OFF\n")); 
  answered += ([ who: 0, ]);

  // Mark the player as having incorrectly answered
  players[who, 2] = 0;
  return 1;
}

void reset_last_result(){
  string* names = m_indices(players);
  for(int i=0; i<sizeof(names); i++){
    players[names[i], 2] = 0;
  }
}

// Give a player some points

int do_add(string arg)
{
  string who;
  int howmuch;

  if (!arg){
    return notify_fail("Syntax: 'trivia add <player> <amount>'\n");
  }

  if(sscanf(arg, "%s %d", who, howmuch) != 2)
  {
    write("TriviaBot tells you: Syntax: trivia add <player> <amt>\n");
    return 1;
  }

  if(!member(players, who))
  {
    write("TriviaBot tells you: That person isn't playing!\n");
    return 1;
  }

  if(!howmuch || howmuch < 0)
  {
    write("TriviaBot tells you: Try trivia subtract "+who+" "+howmuch+"\n");
    return 1;
  }

  players[who, 0] += howmuch;
  say(GAMEMASTER->query_name()+" adds "+howmuch+" points to "
    +capitalize(who)+"'s score!\n");
  return 1;
}

// Remove points from a player

int do_subtract(string arg)
{
  string who;
  int howmuch;

    if (!arg){
    return notify_fail("Syntax: 'trivia add <player> <amount>'\n");
  }

  if(sscanf(arg, "%s %d", who, howmuch) != 2)
  {
    write("TriviaBot tells you: Syntax: trivia subtract <player> <amt>\n");
    return 1;
  }

  if(!member(players, who))
  {
    write("TriviaBot tells you: That person isn't playing!\n");
    return 1;
  }

  if(!howmuch || howmuch < 0)
  {
    write("TriviaBot tells you: Try trivia add "+who+" "+howmuch+"\n");
    return 1;
  }

  players[who, 0] -= howmuch;
  say(GAMEMASTER->query_name()+" subtracts "+howmuch+" points from "
    +capitalize(who)+"'s score!\n");
  return 1;
}


// Repeat the quesiton / time remaining
void do_question()
{
  say("The question is: "+curr_question+"\n");
  say("You have "+find_call_out("end_round")+" seconds left to answer.\n");
}

// Kill trivia
int quit_trivia()
{  
  // Byeeeeee
  destroy_game();
  return 1;
}

void end_round()
{
  in_round = 0;

  say(parse_ansi("\n$BOLD$REDTIME'S UP!$OFF\n\n"));

  award_points();

}

// Figure out who got it right...
int award_points()
{
  // If OT, skip point awarding and score the Sudden Death round
  if (is_overtime){
    score_overtime();
    return 1;
  }

  int total_players,          // Total number of player
      award_amount,
      i;

  string *names,              // All players names
         *winners;            // Just the winners names



  names = m_indices(players);
  total_players = sizeof(names);
  if(!answered) answered = ([ ]);
  winners = ({ });

  // Show everyone the answer
  say(parse_ansi(sprintf(
    "$BOLD%'-'79s\n"
    +"$BOLDThe answer was:  $WHITE"
    +capitalize(letter_answer)+")"
    +"$CYAN "+full_answer+"$WHITE\n"
    +"$BOLD%'-'79s$OFF\n", "-", "-")));

  // Find out got it right
  for(i=0; i<total_players; i++)
  {
    if(member(answered, names[i]) && answered[names[i]] == 1)
    {
      winners += ({ names[i], });
    }
  }

  if(!sizeof(winners))
  {
    say(parse_ansi("$BOLDNo one got that one right!$OFF\n"));
    answered = ([ ]);
    check_points();
    return 1;
  }

  // Award the winnings to the winners
  // Also add to the total_awarded count
  for(i=0; i<sizeof(winners); i++)
  {
    total_points += POINT_AWARD;
    players[winners[i], 0] += POINT_AWARD;
  }

  // Announce the winnings
  string winner_string = _implode(winners, "$WHITE,$CYAN ", "$WHITE and$CYAN ");
  say(parse_ansi("$BOLD$YELLOW"+award_amount+"$WHITE points awarded to $CYAN"
    + winner_string + "$WHITE.$OFF\n"));

  answered = ([ ]);

  check_points();

  return 1;
}

/* Game will now end if a player (or players) points are
*  >= MAX_POINTS , OR if the current round > MAX_ROUNDS
*  in the latter case, the leader is declared the winner. */

int check_points()
{
  // if (sizeof(players) < 4){
  //   say(parse_ansi("$BOLDNot enough players to continue!$OFF\n"));
  //   set_automated("off");
  //   quit_trivia();
  //   }

  string *names,
         *winners;

  int     leader_score = -1;

  object     who;

  names = m_indices(players);
  winners = ({ });

  for(int i=0; i<sizeof(names); i++)
  {
    // If we've reached max rounds, make a note of who's
    // the winner / leader / tied
    if (round==MAX_ROUNDS){
      if (leader_score<=players[names[i], 0]){
        leader_score = players[names[i], 0];
        winners += ({ names[i] });
      }
      else {
        who = find_player(names[i]);
        tell_object(who, parse_ansi(
          "$BOLDSorry, you $REDlost$WHITE. Better luck next time!\n"));
      }
    }

    // If a player (or players) gets over 200 points, end the game
    if(players[names[i], 0] >= MAX_POINTS)
    {
      winners += ({ names[i], });
      game_over++;
    }
  }

  // If MAX_POINTS or MAX_ROUNDS are met, check to see if we have a winner
  if(game_over || round==MAX_ROUNDS){
    // If the combined scores of all player is not > MAX_POINTS, there is no winner.
    if (total_points < MAX_POINTS){
      say(parse_ansi("\n$BOLDMinimum point threshold not met! There is no winner!$OFF\n"));
      destroy_game();
    }

    // Otherwise, find out who won / tied
    else {
      end_game(winners);
    }
    
    return 1;
  }


  // Otherwise, increment round number and begin a new round
  round++;
  
  if (is_automated() && !game_over){
    start_round_automation();
  }
  

  return 1;
}

// See who won, and end the game (unless there's a tie)
int end_game(string *winners)
{
  int i,
      highest;

  string final_winner,
         output,
         tie_winners = "";


  if(sizeof(winners) > 1)
  {
    final_winner = "tie";
  }
  else
    final_winner = winners[0];

  // If we have a tie...
  if(final_winner == "tie")
  {
    say(parse_ansi(
      "$BOLDWE HAVE A $CYANTIE$WHITE!$OFF\n"));
    players = ([]);
    // m_delete(players, who);
    for(i=0; i<sizeof(winners); i++)
    {
      players += ([ winners[i]: 0; 0; 0,]);
      winners[i] = capitalize(winners[i]);
    }
    
    tie_winners = _implode(winners, ", ", " and ");
    shout(parse_ansi(
     "\n$BOLD$REDS U D D E N  D E A T H$WHITE\n"
     +"$BOLD$CYAN"+tie_winners+" $WHITEhave tied in $GREENTrivia!!$OFF\n"));
    say(parse_ansi(
      "\n$BOLD$REDSUDDEN DEATH$WHITE WILL BEGIN IN $GREEN"
      +OT_WARMUP_TIME/60+"$WHITE MINUTE!$OFF\n"
      +"In sudden death, you must answer questions correctly to stay in the\n"
      +"game. If a player answers incorrectly, and their opponent answers\n"
      +"correctly, the player will lose. This rule also applies to non-answers,\n"
      +"as not answering a question is equal to answering incorrectly. However,\n"
      +"if both players fail to answer, the round is pushed.\n\n"
      +"$BOLDGOOD LUCK!$OFF\n"));
      
      // Re-enable automation, enable overtime, reset the round, update questions
      is_overtime = 1;
      round = 1;
      update_api();
      call_out("alert_overtime", (OT_WARMUP_TIME/3*2));
      return 1;
  }
    // If there's only one winner..
    else{
      say(parse_ansi(
        "$BOLDThe winner is: $CYAN"+capitalize(final_winner)+
        "$WHITE!\n"));
      shout(
        "The Trivia game has ended. "+capitalize(final_winner)+
        " is the champion!\n");
  }

  say("Congratulations! And thank you all for playing Dune Trivia!\n");
  game_over = 0;
  do_score();
  game_over = 1;

  destroy_game();
  return 1;
}

// Check that we have enough players (doesn't apply to rounds
// 20 and after or if we're in Sudden Death)
int player_check(){
  if ((sizeof(players)<3 && round < 20) && !is_overtime){
    return 0;
  }
  return 1;
}

// Kill the game (die die die)
void destroy_game(){
  say(parse_ansi(
    "$BOLD$REDTriviaBot$OFF emits a loud $BOLD$YELLOWbeep$OFF and self-destructs, "
    +"$UNDERL$BOLDE$REDX$WHITEP$REDL$WHITEO$REDD$WHITEI$REDN$WHITEG$OFF into bits!\n"));
  // Kill the API
  call_other(API, "clean_up", 1);
  destruct(this_object());
}


// Sudden Death alert
void alert_overtime(){
  
  say(parse_ansi(
    "\n$BOLD$REDSUDDEN DEATH$OFF WILL BEGIN IN $GREEN"
    +OT_WARMUP_TIME/3+"$WHITE SECONDS!$OFF\n\n"));

  // Grab a fresh set of questions from OpenTriviaDB
  if (!set_questions()){
    return;
  }
  tell_object(GAMEMASTER,"TOTAL QUESITONS: " + sizeof(new_questions) + "\n");
  

  call_out("warn_overtime", OT_WARMUP_TIME/4);
}

// Sudden Death warning
void warn_overtime(){
  say(parse_ansi(
    "\n$BOLD$REDSUDDEN DEATH$OFF WILL BEGIN IN $GREEN"
    +OT_WARMUP_TIME/12+"$WHITE SECONDS!$OFF\n\n"));
  
  call_out("do_round", OT_WARMUP_TIME/12);

}

// Score a Sudden Death round
int score_overtime(){
  string  *names = m_indices(players),    // Who's in OT?
          *removed = ({}),                // Who got it wrong / didn't answer?
          losers;                         // Formatted string of losers

  // If someone answered wrong, remove them from the playerlist
  // and append their name to the removed list
  for (int i = 0; i<sizeof(names); i++){
    if (players[names[i], 2] == 0){
      removed+=({names[i]});
      m_delete(players, names[i]);
    }
  }

  // We have a winner!
  if (sizeof(players)==1){
    names = m_indices(players);
    losers = _implode(removed, "$WHITE,$CYAN ", "$WHITE and$CYAN ");
    shout(parse_ansi(
      "\n$BOLD$GREEN"+upper_case(names[0])
      +"$WHITE HAS CRUSHED $CYAN"+upper_case(losers)
      +" $WHITEIN $REDSUDDEN DEATH TRIVIA$WHITE!$BOLD\n"));
    destroy_game();
    return 1;
  }

  // If there are no players left, everyone got it wrong, so its a push
  else if (sizeof(players)==0){
    for (int i=0;i<sizeof(names);i++){
      players += ([names[i]: 0; 0; 0,]);
    }
    say(parse_ansi("$BOLDThis round is a push!\n\n"));
  }

  // Tell the room who got removed
  else {
    for (int i = 0; i < sizeof(removed); i++)
    {
      say(parse_ansi("$BOLD$CYAN" + removed[i] 
      +"$WHITE HAS BEEN $REDELIMINATED$WHITE FROM $REDSUDDEN DEATH$WHITE!"));
    }
    
  }
  
  // Begin the next Sudden Death round
  round++;
  start_round_automation();
  return 1;
}


// Automate the game.
void start_game_automation(){
  if (is_automated()){
    // Make a call to the API
    update_api();
    shout(parse_ansi("\n\n$BOLDTrivia is starting in $GREEN"
      +WARMUP_TIME+"$OFF$BOLD seconds!$OFF\n"
      +"$BOLDHead to the Trivia room (up from Caladan AP) and type:\n"
      +"'$CYANtrivia join$WHITE' "
      +"to join!\n\n$OFF"));

    call_out("warn_game_automation", to_int(WARMUP_TIME*0.834));
  }
}

// Prepare our questions while warning trivia is starting
void warn_game_automation(){
  if (is_automated()){
    shout(parse_ansi("\n\n$BOLDTrivia is starting in $GREEN"
      +WARMUP_TIME/6+"$OFF$BOLD seconds!$OFF\n"
      +"$BOLDHead to the Trivia room (up from Caladan AP) and type:\n"
      +"'$CYANtrivia join$WHITE' "
      +"to join!\n\n$OFF"));
      
    // Get the questions from the API
    if (!set_questions()){
      return;
    }

    // If no questions are loaded, kill trivia
    if(sizeof(new_questions) < 1){
      tell_room(environment(this_object()),
        "Question list empty! API Error! Please Mudmail Bebop!\n");
      return;
  }
    call_out("finish_game_automation", WARMUP_TIME/6);
  }
}

//&& sizeof(players) > 2)

// Check that we have enough players, then start the game
void finish_game_automation(){
  if(is_automated() && player_check()) {
    do_round();
  }

  else {
    tell_room(environment(this_object()), 
      parse_ansi("$BOLD\nNot enough players joined!\n"
      +"$REDAborting$WHITE game!$OFF\n"));
    destroy_game();
  }
}


// Automate the rounds.

void start_round_automation(){
  if (is_automated()){
    say(parse_ansi("\n\n$BOLDRound$RED "+round+"$WHITE starting in $GREEN"
      +BREAK_TIME+"$OFF$BOLD seconds!$OFF\n"));
    call_out("warn_round_automation", to_int(BREAK_TIME*0.834));
  }
}

void warn_round_automation(){
  if (is_automated()){
    say(parse_ansi("\n$BOLDRound$RED "+round+"$WHITE starting in $GREEN"
      +BREAK_TIME/6+"$OFF$BOLD seconds!$OFF\n"));
    call_out("finish_round_automation", BREAK_TIME/6);
  }
}

// Make sure we have enough players to continue
void finish_round_automation(){
  if(is_automated()&& player_check()) {
    do_round();
  }
  else {
    tell_room(environment(this_object()), 
      "$BOLDNot enough players to continue! Shutting down.\n");
    destroy_game();
  }
}

void set_automated(int state){
  automated = state;
}


int is_automated(){
  return automated;
}


int register() { 
  if ("/obj/cmds/player/tell"->add_tell_handler("triviabot",
    this_object())){ 
    return "/obj/cmds/player/tell"->add_tell_handler("trivia",
      this_object()); 
  }
  else return notify_fail(
    "Unable to set tell handler. Please consult a wizard.\n");
}

// Process tells

int hear_tell(string user, string mesg, int reply){ 
  string      who = lower_case(user->query_name());
  if(!present(find_player(who), environment(this_object()))){
    return 0;
  }
  map(filter_objects(users(), "id", getuid()), #'tell_object,
    sprintf("%#Q::hear_tell(%#Q, %#Q, %#Q);\n", this_object(), user, mesg, reply));
    do_answer(who, mesg);
    if(find_player(who)) find_player(who)->add_sp(1);
  return 1;
  }

// Update the API's data
void update_api(){
  //load_object(API)->update_trivia_data();
  
}

// Set our question mapping to the API's data
int set_questions(){
  // new_questions = load_object(API)->query_trivia_data();
  new_questions = load_object(API)->get_dummy_data();
  if(sizeof(new_questions) < 1){
    tell_room(environment(this_object()),
      "Trivia cancelled! Question list empty! Please MudMail Bebop!\n");
  return 0;
  }
  else return 1;
}

// Some getters

int get_num_players(){
  return sizeof(players);
}

int get_total_points(){
  return total_points;
}

int set_round(int i){
  round = i;
  return round;
}

mapping get_players(){
  return players;
}

string did_answer(string who){
  return players[who, 2];
}

int set_points(string who, int val){
  players[who, 0] = val;
  return players[who, 0];
}