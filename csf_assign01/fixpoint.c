#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "fixpoint.h"
#include <cstring>

////////////////////////////////////////////////////////////////////////
// Helper functions
// Note that you can make these "visible" (not static)
// if you want to be able to write unit tests for them
////////////////////////////////////////////////////////////////////////

// TODO: add helper functions

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
    
    uint64_t frac_sum = (uint64_t)left->frac + (uint64_t)right->frac; 
    uint32_t result_frac = (uint32_t)(frac_sum & ((1ULL << 32) - 1));
    uint32_t carry = frac_sum >> 32;

    uint64_t whole_sum = (uint64_t)left->whole + (uint64_t)right->whole + carry;
    uint32_t result_whole = (uint32_t)(whole_sum & ((1ULL << 32) - 1));
    
    result->whole = result_whole;
    result->frac = result_frac;
    result->negative = left->negative;

    if (whole_sum >> 32) {
      return RESULT_OVERFLOW;
    }
    else{
      return RESULT_OK;
    }

  } else {
    if (left->whole == right->whole){ //case 2, same #
      if(left->frac == right->frac){
      result->frac = 0;
      result->whole = 0;
      result->negative = false;
      return RESULT_OK;
      }
      if(left->frac > right->frac){ //case 2.1 left frac greater
      result->frac = left->frac - right->frac;
      result->whole = 0;
      result->negative = left->negative;
      }
      if(right->frac > left->frac){ //case 2.2 right frac greater
      result->frac = right->frac - left->frac;
      result->whole = 0;
      result->negative = right->negative;
      }
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
        result->frac = (uint64_t)left->frac + (1ULL << 32) - right->frac;
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
      else{ //case 6: right whole > left, left frac < right
        result->whole = right->whole - left->whole - 1;
        result->frac = (uint64_t)right->frac + (1ULL << 32) - left->frac;
        result->negative = right->negative;
        return RESULT_OK;
      }
    }
  }
}

result_t
fixpoint_sub( fixpoint_t *result, const fixpoint_t *left, const fixpoint_t *right ) {
  // take add function from above and negate the right side 
  // a - b is same as a + (-b)
  fixpoint_t negated_right = *right;
  fixpoint_negate(&negated_right);
  return fixpoint_add(result, left, &negated_right);
}

result_t
fixpoint_mul( fixpoint_t *result, const fixpoint_t *left, const fixpoint_t *right ) {
  uint64_t left_val = ((uint64_t) left->whole << 32) | left->frac;
  uint64_t right_val = ((uint64_t) right->whole << 32) | right->frac;

  uint128_t multiplication = (uint128_t) left_val * (uint128_t) right_val; // 128-bit product representation
  uint64_t middle = (uint64_t) (multiplication << 32); // take the middle 64

  // here shift middle 64 bits into result frac/whole and determine negativity
  result->whole = (uint32_t) (middle >> 32);
  result->frac = (uint32_t) (middle << 32);
  result->negative = (left->negative != right->negative);

  uint32_t low = (uint32_t) (multiplication >> 96);
  uint32_t high = (uint32_t) (multiplication << 96);
  if (high) {
    return RESULT_OVERFLOW; // high 32 bits not 0
  }
  if (low) {
    return RESULT_UNDERFLOW; // low 32 bits not 0
  }

  if (result->whole == 0 && result->frac == 0) {
    result->negative = false; // false because 0 isn't negative
  }

  return RESULT_OK;
}

int
fixpoint_compare( const fixpoint_t *left, const fixpoint_t *right ) {
  if(left->whole > right-> whole){
    return 1;
  }
  if(left->whole < right->whole){
    return -1;
  }
  if(left->whole == right->whole){
    if(left->frac > right->frac){
      return 1;
    }
    if(left->frac < right->frac){
      return -1;
    }
    if(left->frac == right->frac){
      return 0;
    }
  }
}

void
fixpoint_format_hex( fixpoint_str_t *s, const fixpoint_t *val ) {
    char whole[32];
    char fraction[32];
    if(val->whole == 0){
      strcpy(whole_str, "0");
    } else {
      snprint(whole, sizeof(whole_str), "%X", val->whole);
    }

    snprintf(fraction, sizeof(frac_str), "%08X", val->frac);
    while(strlen(fraction) > 1 && fraction[strlen(fraction) - 1]) == '0'){
      fraction[strlen(fraction) - 1] = '\0';
    }

    if (val->negative){
      snprintf(s->buf, sizeof(s->buf), "-%s.%s", whole_str, frac_str);
    } else {
        snprintf(s->buf, sizeof(s->buf), "%s.%s", whole_str, frac_str);
    }

}

bool
fixpoint_parse_hex( fixpoint_t *val, const fixpoint_str_t *s ) {
  // TODO: implement
}
