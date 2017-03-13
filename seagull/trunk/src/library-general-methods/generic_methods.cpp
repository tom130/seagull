/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * (c)Copyright 2006 Hewlett-Packard Development Company, LP.
 *
 */

#include "generic_methods.hpp"
#include "iostream_t.hpp"
#include <regex.h>
#include "string_t.hpp"
#include "Utils.hpp"
#include <time.h>
#include <sstream>




char* external_find_text_value (char *P_buf, char *P_field) {

  if ((P_buf == NULL) || (P_field == NULL))
    return NULL;

  char *L_value = NULL ;


  regex_t    L_reg_expr ;
  int        L_status ;
  char       L_buffer[100];
  regmatch_t L_pmatch[3] ;
  size_t     L_size = 0 ;

  string_t   L_string = "" ;

  L_string  = "([[:blank:]]*" ;
  L_string += P_field ;
  L_string += "[[:blank:]]*=[[:blank:]]*)([ *$]+)";

  L_status = regcomp (&L_reg_expr,
                      L_string.c_str(),
                      REG_EXTENDED) ;

  if (L_status != 0) {
    regerror(L_status, &L_reg_expr, L_buffer, 100);
    regfree (&L_reg_expr) ;
  } else {

    L_status = regexec (&L_reg_expr, P_buf, 3, L_pmatch, 0) ;
    regfree (&L_reg_expr) ;
    if (L_status == 0) {
      L_size = L_pmatch[2].rm_eo - L_pmatch[2].rm_so ;
      ALLOC_TABLE(L_value, char*, sizeof(char), L_size+1);
      memcpy(L_value, &(P_buf[L_pmatch[2].rm_so]), L_size);
      L_value[L_size]='\0' ;
    }
  }
  return (L_value);
}

typedef struct args_string {
  char * m_startoffset;
  char * m_session;

} T_ArgsStr, *T_pArgsStr ;

static const T_ArgsStr Args_Str_init = {
  (char *)"0"
} ;


int args_analysis (T_pValueData  P_args, T_pArgsStr P_result) {

  int             L_ret = 0 ;

  *P_result = Args_Str_init ;
  P_result->m_startoffset = external_find_text_value((char*)P_args->m_value.m_val_binary.m_value,
                                             (char*)"startoffset")  ;
  P_result->m_session = external_find_text_value((char*)P_args->m_value.m_val_binary.m_value,
                                               (char*)"session")  ;

  if (P_result->m_startoffset == NULL)
  P_result->m_startoffset = (char *)"0";

  if (P_result->m_session == NULL)
  P_result->m_session = (char *)"0";
  return (L_ret);
}

int sys_time_secs (T_pValueData  P_msgPart,
                   T_pValueData  P_args,
                   T_pValueData  P_result) {

  int             L_ret    = 0    ;


   T_ArgsStr L_args;
  (void)args_analysis (P_args, &L_args);
  P_result->m_type = E_TYPE_SIGNED ;
  P_result->m_value.m_val_signed = time(NULL) + atol(L_args.m_startoffset);

   FREE_TABLE(L_args.m_startoffset);
   return (L_ret);



}

int session_id (T_pValueData  P_msgPart,
                   T_pValueData  P_args,
                   T_pValueData  P_result) {
/*
 Input
 ------------
 session=string_session

 Returns
 ------------
 string_session+time(HHMMSS)+rand()
*/


  int             L_ret    = 0    ;

  time_t current_time;
  struct tm * time_info;
  char timeString[7];  // space for "HHMMSS\0"
  char L_result [150];
  int random;
  time(&current_time);
  time_info = localtime(&current_time);
  std::ostringstream out;
  std::ostringstream out_rand;

  //Time part
  strftime(timeString, sizeof(timeString), "%H%M%S", time_info);
  srand (time(NULL));
  random = rand() % 1000 + 1;
  //String conversion
  out_rand << random;
  std::string rand_str=out_rand.str();

   T_ArgsStr L_args;
  (void)args_analysis (P_args, &L_args);
  //returning string
  P_result->m_type = E_TYPE_STRING ;

  // Return part
  out << L_args.m_session << timeString << rand_str ;
  std::string out2=out.str();
  ALLOC_TABLE(P_result->m_value.m_val_binary.m_value,
                    unsigned char*,
                    sizeof(unsigned char),
                                        strlen(L_result));

                strncpy(L_result, out2.c_str(), sizeof(L_result));

        P_result->m_value.m_val_binary.m_size = strlen(L_result);
        memcpy(P_result->m_value.m_val_binary.m_value, L_result, strlen(L_result));
        FREE_TABLE(L_args.m_session);
        return (L_ret);

}



int sys_time_unsig_sec (T_pValueData  P_msgPart,
                   T_pValueData  P_args,
                   T_pValueData  P_result) {

  int             L_ret    = 0    ;



   T_ArgsStr L_args;


  (void)args_analysis (P_args, &L_args);
  P_result->m_type = E_TYPE_NUMBER  ;
  P_result->m_value.m_val_signed = time(NULL) + atol(L_args.m_startoffset);
  FREE_TABLE(L_args.m_startoffset);
  return (L_ret);
}

