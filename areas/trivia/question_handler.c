/*
 * Trivia Question Generator
 * modified Dox's example to better serve trivia
 *   - Paradox@DUNE - 2021-07-04
 *   - Bebop@DUNE - 2021-08-01
 */
#pragma strict_types, save_types, rtt_checks, pedantic, no_shadow, no_clone

#define DEBUG_TARGET "bebop"
#define DEBUG_OFF
// #include <debug.h>

#include "/secure/obj/py_http/py_http.h"

inherit "/secure/obj/py_http/py_http_types";

#define PY_HTTP_CLIENT "/secure/obj/py_http/py_http_client.c"
#define API_BASE_URL "https://opentdb.com/api.php"

string* triviaData = ({});

public void question_handler(struct HTTPResponse resp) {
  mapping questionData;
  mixed data = json_parse(resp->body);

  /* Build a string array of questions / answers for trivia
  *  string format for trivia:
  *  <question>#<correct_answer>#<all_answers>
  *  the <all_answers> bit is shuffled.
  *  if no API results are found, return empty array (handled in trivia.c)
  */

  if(sizeof(data["results"]) > 0) {
    for (int i=0; i < sizeof(data["results"]); i++){
      questionData = data["results"][i];
      triviaData +=  ({ questionData["question"] + "#" + questionData["correct_answer"] 
        + "#" + _implode(m_indices(mkmapping(({questionData["correct_answer"]}) 
        + questionData["incorrect_answers"])), ", ", " or ") });
    }
  }

  else {
    write("No results returned!");
  }

}

public int error_handler(){
  return notify_fail("Trivia API Error!\nContact someone who knows more than Bebop! (anyone)\n");
}

// Grab 50 questions at random. Any difficulty, any category.

public void get_questions(closure success, closure error) {
  success ||= #'question_handler;
  error ||= #'error_handler;

  mapping url_args = ([ "amount": 50 ]);
  
  // Uncomment to specify a category / difficulty
  // url_args["category"] = "General Knowledge";
  // url_args["difficulty"] = 2;

  load_object(PY_HTTP_CLIENT)->get_json(API_BASE_URL, url_args, 0, success, error);
}

public string* update_trivia_data(){
  triviaData = ({ });
  get_questions(#'question_handler, #'error_handler);
}

public string* query_trivia_data(){
  return triviaData;
}