#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "fixpoint.h"

////////////////////////////////////////////////////////////////////////
// Helper functions
// Note that you can make these "visible" (not static)
// if you want to be able to write unit tests for them
////////////////////////////////////////////////////////////////////////

// TODO: add helper functions

/*int count_digs(int n) {
  if (n == 0) {
    return 1;
  }
  if (n < 0) {
    n = -n;
  }

  int count = 0;
  while (n > 0) {
    n /= 10;
    count++;
  }
  return count;
}*/

////////////////////////////////////////////////////////////////////////
// Public API functions
////////////////////////////////////////////////////////////////////////

void
fixpoint_init( fixpoint_t *val, uint32_t whole, uint32_t frac, bool negative ) {
  val->whole = whole;
  val->frac = frac;
  if (whole == 0 && frac == 0) {
    val->negative = false; // zero never negative
  } else {
    val->negative = negative;
  }
}

uint32_t
fixpoint_get_whole( const fixpoint_t *val ) {
  return val->whole;
}

uint32_t
fixpoint_get_frac( const fixpoint_t *val ) {
  return val->frac;
}

bool
fixpoint_is_negative( const fixpoint_t *val ) {
  return val->negative;
}

void
fixpoint_negate( fixpoint_t *val ) {
  if(val->frac != 0 || val->whole != 0){
    val->negative = !(val->negative);
  }
  else {
    val->negative = 0;
  }
}

result_t
fixpoint_add( fixpoint_t *result, const fixpoint_t *left, const fixpoint_t *right ) {
  //check if signs are same
  //if same then just add all the values and keep same sign
  //if different then find bigger whole and subtract smaller from bigger number
  //if wholes are both zero then do that with the fractions
  /*if (left->negative == right->negative) { // case 1: same sign
    uint32_t frac_add = left->frac + right->frac;
    // check if frac_add overflows 
    uint32_t carry;
    uint32_t whole_add = left->whole + right->whole + carry; 
    result->frac = frac_add;
    result->whole = whole_add;
    if ( > UINT32_MAX) {
      return RESULT_OVERFLOW;
    } else {
      return RESULT_OK
    }
    //if overflow/ok
  } else { // case 2: different sign
    if (left->whole < right->whole) {
      // if the right fraction bigger than left then you are good
      if (right->frac > left->frac) { // case 2.1: right fraction larger
        result->frac = right->frac - left->frac;
        result->whole = right->whole - left->whole;
      } else { // case 2.2: left fraction larger
        result->whole = right->whole - left->whole - 1;
        if ()
        count_digs(result)
      }
      // if not then you have to account for carry

    }
  }*/
}

result_t
fixpoint_sub( fixpoint_t *result, const fixpoint_t *left, const fixpoint_t *right ) {
  // TODO: implement
  // take add function from above and negate the right side 
  // a - b is same as a + (-b)
  /*
  fixpoint_t negated_right = *right;
  fixpoint_negate(&negated_right);
  return fixpoint_add(result, left, &negated_right);
  */
}

result_t
fixpoint_mul( fixpoint_t *result, const fixpoint_t *left, const fixpoint_t *right ) {
  // TODO: implement
}

int
fixpoint_compare( const fixpoint_t *left, const fixpoint_t *right ) {
  // TODO: implement
}

void
fixpoint_format_hex( fixpoint_str_t *s, const fixpoint_t *val ) {
  // TODO: implement
}

bool
fixpoint_parse_hex( fixpoint_t *val, const fixpoint_str_t *s ) {
  // TODO: implement
}
