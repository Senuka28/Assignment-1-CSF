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
  
  if (left->negative == right->negative) { //Case 1: signs same
    
    
    
    uint64_t frac_sum = left->frac + right->frac; 
    uint64_t result_frac = frac_sum & ((1 << 32) - 1);
    uint32_t carry = frac_sum >> 32;

    uint64_t whole_sum = left->whole + right->whole + carry;
    uint32_t result_whole = whole_sum & ((1 << 32) - 1);
    
    result->whole = result_whole;
    result->frac = result_frac;
    result->negative = left->negative;

    if (whole_sum > UINT32_MAX) {
      return RESULT_OVERFLOW;
    }
    else{
      return RESULT_OK;
    }

  } else {
    if (left->whole == right->whole && left->frac == right->frac){ //case 2, same #
      result->frac = 0;
      result->whole = 0;
      result->negative = false;
      return RESULT_OK;
    }
    if (left->whole > right->whole){ 
      if(left->frac >= right->frac){ //case 3: left whole and frac > right
        result->whole = left->whole - right->whole;
        result->frac = left->frac - right->frac;
        result->negative = left->negative;
        return RESULT_OK;
      }
      else{ //case 4: left whole > right, left frac < right
        result->whole = left->whole - right->whole - 1;
        result->frac = left->frac + 1 - right->frac;
        result->negative = left->negative;
        return RESULT_OK;
      }
    }
    if (right->whole > left->whole){ //case 5: right whole and frac > left
      if(right->frac >= left->frac){
        result->whole = right->whole - left->whole;
        result->frac = right->frac - left->frac;
        result->negative = right->negative;
        return RESULT_OK;
      }
    }


  }
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
