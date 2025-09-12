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

typedef struct {
 uint64_t hi;
 uint64_t lo;
} uint128_sim;

static uint128_sim mul64(uint64_t a, uint64_t b) {
  uint128_sim res;

  // split a and b
  uint32_t a_hi = (uint32_t)(a >> 32);
  uint32_t a_lo = (uint32_t)(a & 0xFFFFFFFF);
  uint32_t b_hi = (uint32_t)(b >> 32);
  uint32_t b_lo = (uint32_t)(b & 0xFFFFFFFF);

  // find partial products with above vals
  uint64_t product0 = (uint64_t)a_lo * b_lo;
  uint64_t product1 = (uint64_t)a_lo * b_hi; 
  uint64_t product2 = (uint64_t)a_hi * b_lo; 
  uint64_t product3 = (uint64_t)a_hi * b_hi; 

  // accumulate the results into 128 bits
  uint64_t middle = (product0 >> 32) + (product1 & 0xFFFFFFFF) + (product2 & 0xFFFFFFFF);

  // now finally assign the result
  res.lo = (product0 & 0xFFFFFFFF) | (middle << 32);
  res.hi = product3 + (product1 >> 32) + (product2 >> 32) + (middle >> 32);

  return res;
}



////////////////////////////////////////////////////////////////////////
// Public API functions
////////////////////////////////////////////////////////////////////////

// Initialize fixpoint value with whole, fractional, and sign fields.
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

// Return the whole part of a fixpoint value.
uint32_t
fixpoint_get_whole( const fixpoint_t *val ) {
  return val->whole;
}

// Return the fractional part of a fixpoint value.
uint32_t
fixpoint_get_frac( const fixpoint_t *val ) {
  return val->frac;
}

// Return true if the fixpoint value is negative.
bool
fixpoint_is_negative( const fixpoint_t *val ) {
  return val->negative;
}

// Negate a fixpoint value (unless it is 0).
void
fixpoint_negate( fixpoint_t *val ) {
  if(val->frac != 0 || val->whole != 0){
    val->negative = !(val->negative);
  }
  else {
    val->negative = 0;
  }
}

// Add two fixpoint values, stores the result and reports overflow/underflow.
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
      return RESULT_OK;
      }
      if(right->frac > left->frac){ //case 2.2 right frac greater
      result->frac = right->frac - left->frac;
      result->whole = 0;
      result->negative = right->negative;
      return RESULT_OK;
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
  return RESULT_OK;

}

// Subtract right from left by negating right and adding.
result_t
fixpoint_sub( fixpoint_t *result, const fixpoint_t *left, const fixpoint_t *right ) {
  // take add function from above and negate the right side 
  // a - b is same as a + (-b)
  fixpoint_t negated_right = *right;
  fixpoint_negate(&negated_right);
  return fixpoint_add(result, left, &negated_right);
}

// Multiply two fixpoint values, stores the result and reports overflow/underflow.
result_t
fixpoint_mul( fixpoint_t *result, const fixpoint_t *left, const fixpoint_t *right ) {
  // TODO: implement
  uint64_t left_val = ((uint64_t) left->whole << 32) | left->frac;
  uint64_t right_val = ((uint64_t) right->whole << 32) | right->frac;

  uint128_sim multiplication = mul64(left_val, right_val); // 128-bit product representation
  uint64_t middle = (multiplication.hi << 32) | (multiplication.lo >> 32); // take the middle 64


  // here shift middle 64 bits into result frac/whole and determine negativity
  result->whole = (uint32_t) (middle >> 32);
  result->frac = (uint32_t) (middle & 0xFFFFFFFF);
  result->negative = (left->negative != right->negative);

  if (multiplication.hi > 0xFFFFFFFF && (multiplication.lo & 0xFFFFFFFF) != 0) {
    return RESULT_OVERFLOW | RESULT_UNDERFLOW;
  }
  if (multiplication.hi > 0xFFFFFFFF) {
    return RESULT_OVERFLOW; // high 32 bits not 0
  }
  if ((multiplication.lo & 0xFFFFFFFF) != 0) {
    return RESULT_UNDERFLOW; // low 32 bits not 0
  }

  if (result->whole == 0 && result->frac == 0) {
    result->negative = false; // false because 0 isn't negative
  }


  return RESULT_OK;
}

// Compare two fixpoint values: returns -1 if left < right, 0 if equal, 1 if left > right.
int
fixpoint_compare( const fixpoint_t *left, const fixpoint_t *right ) {
  
  // if one number is negative and other isn't
  if (left->negative && !right->negative){
    return -1;
  }
  if(!left->negative && right->negative){
    return 1;
  }
  
  // same sign
  if(left->whole > right-> whole){
    return left->negative ? -1 : 1;
  }
  if(left->whole < right->whole){
    return left->negative ? 1 : -1;
  }

  // equal wholes
  if(left->whole == right->whole){
    if(left->frac > right->frac){
      return left->negative ? -1 : 1;
    }
    if(left->frac < right->frac){
      return left->negative ? 1 : -1;
    }
    if(left->frac == right->frac){
      return 0;
    }
  }

  return 0;
}

// Format a fixpoint value as a hexadecimal string.
void
fixpoint_format_hex( fixpoint_str_t *s, const fixpoint_t *val ) {
    char whole[9];
    char fraction[9];
    if(val->whole == 0){
      strcpy(whole, "0");
    } else {
      snprintf(whole, sizeof(whole), "%x", val->whole);
    }

    //remove trailing 0s
    snprintf(fraction, sizeof(fraction), "%08x", val->frac);
    int len = strlen(fraction);
    while(len > 1 && fraction[len - 1] == '0'){
      fraction[--len] = '\0';
    }

    // if fraction is 0
    if (len == 1 && fraction[0] == '0') {
        if (val->negative) {
            snprintf(s->str, sizeof(s->str), "-%s.0", whole);
        } else {
            snprintf(s->str, sizeof(s->str), "%s.0", whole);
        }
    } else {
        if (val->negative) {
            snprintf(s->str, sizeof(s->str), "-%s.%s", whole, fraction);
        } else {
            snprintf(s->str, sizeof(s->str), "%s.%s", whole, fraction);
        }
    }

}

// Parse a hexadecimal string into a fixpoint value, returning true if valid.
bool
fixpoint_parse_hex( fixpoint_t *val, const fixpoint_str_t *s ) {
  if (val == NULL || s == NULL || s->str == NULL) {
        return false;
    }

  unsigned int whole;
  unsigned int fraction;
  const char *str = s->str;

  //handle negative sign
  if(*str == '-'){
    val->negative = true;
    str++;
  }
  else{
    val->negative = false;
  }

  int whole_digits = 0;
  if(sscanf(str, "%8x%n", &whole, &whole_digits) != 1 || whole_digits == 0 || whole_digits > 8){
    return false;
  }
  if (whole_digits > 1 && *str == '0') {   //reject leading zeros unless only 0
    return false;
  }
  val->whole = whole;



  if (str[whole_digits] != '.') {
        return false;
  }

  const char *fraction_str = str + whole_digits + 1; 
  
  if(*fraction_str == '\0'){
    return false;
  }

  int frac_digits = 0;
  if (sscanf(fraction_str, "%8x%n", &fraction, &frac_digits) != 1 || frac_digits == 0 || frac_digits > 8) {
    return false;
  }

  if(frac_digits > 0 && frac_digits <= 8){
    val->frac = fraction << (4 * (8 - frac_digits)); 
  } else {
    val->frac = 0;
  }

  if (fraction_str[frac_digits] != '\0') {   //reject trailing junk after fraction
    return false;
  }

  //no negative 0
  if (val->whole == 0 && val->frac == 0) {
    val->negative = false;
  }
    
  return true;

}