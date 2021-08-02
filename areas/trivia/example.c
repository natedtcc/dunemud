$/obj/py_http>
/*
 * Simple https://opentdb.com/ example
 *   - Paradox@DUNE - 2021-07-04
 *   - Bebop@DUNE
 */
#pragma strict_types, save_types, rtt_checks, pedantic, no_shadow, no_clone

#define DEBUG_TARGET "paradox"
#define DEBUG_ON
#include <debug.h>

#include "py_http.h"

inherit __PATH__(0)"py_http_types";

#define PY_HTTP_CLIENT "/secure/obj/py_http/py_http_client.c"
#define API_BASE_URL "https://opentdb.com/api.php"

public void question_handler(struct HTTPResponse resp) {
  mixed data = json_parse(resp->body);

  if(sizeof(data["results"]) < 1) {
    printf("Didn't get any results!\n");
    return;
  }

  mapping questionData = data["results"][0];

  write("Trivia time!\n");
  printf("Question Type: %s\n", questionData["type"]);
  printf("Question Difficulty: %s\n", questionData["difficulty"]);
  printf("Question Category: %s\n", questionData["category"]);
  printf("Question: %s\n", questionData["question"]);
  write("\n");
  printf("WRONG Answers: %s\n", _implode(questionData["incorrect_answers"], ", ", " and "));
  write("\n");
  printf("CORRECT Answer: %s\n", questionData["correct_answer"]);
}

public void get_question(int category, string difficulty, string type, closure success, closure error) {
  success ||= #'question_handler';

  mapping url_args = ([ "amount": 1 ]);
  if(category > 0) {
    url_args["category"] = category;
  }
  if(stringp(difficulty)) {
    url_args["difficulty"] = difficulty;
  }
  if(stringp(type)) {
    url_args["type"] = type;
  }

  load_object(PY_HTTP_CLIENT)->get_json(API_BASE_URL, url_args, 0, success, error);
}
