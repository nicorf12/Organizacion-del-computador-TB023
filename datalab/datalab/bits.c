/*
 * CS:APP Data Lab
 *
 * <Please put your name and userid here>
 *
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:

  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code
  must conform to the following style:

  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>

  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.


  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 *
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce
 *      the correct answers.
 */


#endif
//1
/* 0110 0101 -> 0100, 1001 1010 -> 1011 -> 0100 
 * bitAnd - x&y using only ~ and |
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
  int z;
  z = ~x | ~y;
  return ~z;
}
/*
 * bitMatch - Create mask indicating which bits in x match those in y
 *            using only ~ and &
 *   Example: bitMatch(0x7, 0xE) = 0x6
 *   Legal ops: ~ & |
 *   Max ops: 14
 *   Rating: 1
 */
int bitMatch(int x, int y) {
  return (x & y) | (~x & ~y);

}
/*
 * bitNor - ~(x|y) using only ~ and &
 *   Example: bitNor(0x6, 0x5) = 0xFFFFFFF8
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */
int bitNor(int x, int y) {
  return (~x & ~y);
}
/*
 * bitXor - x^y using only ~ and &
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  return (~(x & y))&(~(~x & ~y));
}
//2
/*
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
    int m8 = 0xAA;
    int m16 = m8 | m8 << 8;
    int m32 = m16 | m16 << 16; 
    return !((x & m32) ^ m32);
}
/*
 * anyEvenBit - return 1 if any even-numbered bit in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples anyEvenBit(0xA) = 0, anyEvenBit(0xE) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int anyEvenBit(int x) {
  int m8 = 0x55;
  int m16 = m8 | m8 << 8;
  int m32 = m16 | m16 << 16;
  int evenx = x & m32;
  return !!evenx;
}
/*
 * byteSwap - swaps the nth byte and the mth byte
 *  Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *            byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *  You may assume that 0 <= n <= 3, 0 <= m <= 3
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 2
 */
int byteSwap(int x, int n, int m) {
  int ocho_n = n << 3;
  int ocho_m = m << 3;
  int mascara_n = x & (0xFF << ocho_n);
  int mascara_m = x & (0xFF << ocho_m);
  int valor_n = (mascara_n >> ocho_n) & 0xFF;
  int valor_m = (mascara_m >> ocho_m) & 0xFF;
  int m32 = (x & ~(mascara_m | mascara_n)) | (valor_n << ocho_m | valor_m << ocho_n); 
  return m32;
}
/*
 * fitsBits - return 1 if x can be represented as an
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
  int signo = x >> 31; //0 si es neg, -1 sino (todos 1)
  int shift = x >> (n + (~0)); // shifteo n-1, (pierdo n-1 bits menos significativos)
  return !(shift ^ signo); // si son iguales
}

/*
 * negate - return -x
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  int z;
  z= ~x;
  return z+1;
}
/*
 * sign - return 1 if positive, 0 if zero, and -1 if negative
 *  Examples: sign(130) = 1
 *            sign(-23) = -1
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 10
 *  Rating: 2
 */
int sign(int x) {
    int b_sign;
    int is_zero;
    b_sign = x >> 31;
    is_zero = !!x;
    return b_sign | is_zero;
}
//3
/*
 * addOK - Determine if can compute x+y without overflow
 *   Example: addOK(0x80000000,0x80000000) = 0,
 *            addOK(0x80000000,0x70000000) = 1,
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int addOK(int x, int y) {
  int z;
  int sing_z;
  int sing_x;
  int sing_y;
  int xy_mismo_sing;
  int zx_distinto_sing;
  sing_x = x >> 31;
  sing_y = y >> 31;
  z= (x+y);
  sing_z = z >> 31;
  xy_mismo_sing = ~sing_x ^ sing_y;
  zx_distinto_sing = sing_x ^ sing_z;
  return  !( xy_mismo_sing & zx_distinto_sing );
}
/*
 * bitMask - Generate a mask consisting of all 1's
 *   lowbit and highbit
 *   Examples: bitMask(5,3) = 0x38
 *   Assume 0 <= lowbit <= 31, and 0 <= highbit <= 31
 *   If lowbit > highbit, then mask should be all 0's
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int bitMask(int highbit, int lowbit) {
  int mask;
  int complement_mask_h;
  int complement_mask_l;
  int equal_low_high;
  int solution;
  mask = ~0x00;
  
  complement_mask_h = mask<<highbit;
  complement_mask_h = complement_mask_h << 1;
  complement_mask_l = ~(mask<<lowbit);
  solution = ~( complement_mask_h | complement_mask_l );
  equal_low_high = !( (highbit) ^ (~(lowbit)) );
  return solution | (equal_low_high<<lowbit);
}
/*
 * conditional - same as x ? y : z
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  int boolean;
  boolean = !!x;
  boolean = boolean<<31;
  boolean = boolean >> 31;
  return (y & boolean) | (z & ~boolean);
}
/*
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  int cuarenta_y_ocho = 0x30;
  int cincuenta_y_siete = 0x39;
  int m_x;
  int m_y;
  m_x = x + ~cuarenta_y_ocho + 1;
  m_x >>= 31; //si x es mayor a 0x30, el resultado de la resta en ka operacion anterior dio un numero positivo, entonces realizar esta operacion deberia dar como resultado 0
  m_y = cincuenta_y_siete + ~x + 1; 
  m_y >>= 31; //de forma similar, si x esta en el rango de 0x30 a 0x39, entonces es menor o igual a 0x39, por lo que si la resta de la operacion anterior dio un numero positivo, quiere decir que el numero se encuentra en el rango y por ende shiftear 31 veces a derecha deja el numero en 0
  return !m_x & !m_y;
}
/*
 * isGreater - if x > y  then return 1, else return 0
 *   Example: isGreater(4,5) = 0, isGreater(5,4) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isGreater(int x, int y) { 
  int resta = x + ~y;
  int signox = (x >> 31) & 1;
  int signoy = (y >> 31) & 1;
  int signo_resta = (resta >> 31) & 1;
  int mismo_signo = (!(signox ^ signoy)) & signo_resta;
  int signo_distinto = (signox & !signoy);
  return !(mismo_signo | signo_distinto);
}
/*
 * replaceByte(x,n,c) - Replace byte n in x with c
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: replaceByte(0x12345678,1,0xab) = 0x1234ab78
 *   You can assume 0 <= n <= 3 and 0 <= c <= 255
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 3
 */
int replaceByte(int x, int n, int c) {
  int mascara = 0xFF;
  int m32_0 = 0x00 << 24;
  int m32 = m32_0 | mascara << (n << 3);
  int mx = x & ~m32;
  int mc = c << (n << 3);
  mc |= m32_0;
  return mx | mc;
  
}
//4
/*
 * absVal - absolute value of x
 *   Example: absVal(-1) = 1.
 *   You may assume -TMax <= x <= TMax
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int absVal(int x) {
  int sing;
  sing = x >>31;
  return (x & ~sing) | ((~x+1) & sing);
}
/*
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4
 */
int bang(int x) {
  int negX;
  int signBit;
  int mask;
  negX = ~x+1;
  signBit = (x | negX) >> 31;
  mask = signBit & 1;
  return (~mask) & 1;
}
/*
 * isNonZero - Check whether x is nonzero using
 *              the legal operators except !
 *   Examples: isNonZero(3) = 1, isNonZero(0) = 0
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int isNonZero(int x) { 
  int negX = ~x + 1; 
  int shift =(x | negX) >> 31; //solo si x es 0 el or del bit de signo es 0 en x y negX, sino queda 1
  return shift & 1;
} 
/*
 * logicalNeg - implement the ! operator, using all of
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4
 */
int logicalNeg(int x) {
    int negX = ~x + 1;
    int shift = (x | negX) >> 31;  //solo si x es 0 el or del bit de signo es 0 en x y negX, sino queda 1
    return shift + 1; 
}


//float
/*
 * floatAbsVal - Return bit-level equivalent of absolute value of f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument..
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned floatAbsVal(unsigned uf) {
  unsigned mask;
  int is_nan;
  unsigned exp;
  unsigned frac;
  unsigned absVal;
  unsigned mask_frac;
  mask = 0x7FFFFFFF;
  mask_frac = 0x7FFFFF;

  absVal = mask&uf;
  exp = absVal >> 23;
  frac = absVal & mask_frac;
  is_nan = !(exp ^ 0xFF) && frac;
  if (is_nan) {
    return uf;
  }
  return absVal;
  /*
  METODO SIN IF - ELSE
  //is_nan = (!!is_nan) << 31;
  //is_nan = is_nan >>31;
  //return ( (absVal)&(~is_nan) ) | (uf & is_nan);
   */
}
/*
 * floatIsEqual - Compute f == g for floating point arguments f and g.
 *   Both the arguments are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   If either argument is NaN, return 0.
 *   +0 and -0 are considered equal.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 25
 *   Rating: 2
 */
int floatIsEqual(unsigned uf, unsigned ug) {
    unsigned signo;
    unsigned exponente;
    unsigned mantisa; 
    unsigned signo_uf;
    unsigned exponente_uf;
    unsigned mantisa_uf;
    unsigned signo_ug;
    unsigned exponente_ug;
    unsigned mantisa_ug;

    mantisa =  0x7FFFFF;
    exponente = 0x47800000;
    signo = 0x80;
    signo <<= 24;
    signo_uf = uf & signo;
    exponente_uf = uf & exponente;
    mantisa_uf = uf & mantisa;
    signo_ug = ug & signo;
    exponente_ug = ug & exponente;
    mantisa_ug = ug & mantisa;

    if ((exponente_uf == exponente && mantisa_uf)||(exponente_ug == exponente && mantisa_ug)) { // alguno Nan
        return 0;
    }
    if (exponente_uf == 0 && mantisa_uf == 0 && exponente_ug == 0 && mantisa_ug == 0) { 
        return 1;
    }

    return (exponente_uf == exponente_ug && mantisa_uf == mantisa_ug && signo_uf == signo_ug); 
}

/*
 * floatNegate - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned floatNegate(unsigned uf) {
    unsigned signo;
    unsigned exponente;
    unsigned mantisa;
    unsigned exponente_uf;
    unsigned mantisa_uf;

    signo = 0x80;
    signo <<= 24;
    exponente = 0x47800000;
    mantisa =  0x7FFFFF;
    exponente_uf = uf & exponente;
    mantisa_uf = uf & mantisa;

    if (exponente_uf == exponente && mantisa_uf != 0) { // es Nan
        return uf;
    }

    return uf ^ signo; // cambio bit de signo
}

/*
 * floatIsLess - Compute f < g for floating point arguments f and g.
 *   Both the arguments are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   If either argument is NaN, return 0.
 *   +0 and -0 are considered equal.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 3
 */
int floatIsLess(unsigned uf, unsigned ug) {
    int equalSing;
    int lessExp;
    int lessFrac;
    int lessSing;
    unsigned f_exp;
    unsigned f_frac;
    unsigned g_exp;
    unsigned g_frac;
    int maskAbs;
    int maskFrac;
    int is_nan;
    int are_zero;

    maskAbs = 0x7FFFFFFF;
    maskFrac = 0x7FFFFF;
    f_exp = ( maskAbs&uf ) >> 23;
    f_frac = uf & maskFrac; 
    g_exp = ( maskAbs&ug ) >> 23;
    g_frac = ug & maskFrac;
    are_zero = !((uf | ug) << 1);
    is_nan = (f_exp == 0xFF && f_frac) || (g_exp == 0xFF && g_frac);

    if (is_nan || are_zero){
      return 0;
    }

    equalSing = (uf >> 31) == (ug >> 31);
    if (equalSing){
      //positivo
      lessExp = f_exp < g_exp;
      lessFrac = f_frac < g_frac;
      if (uf>>31) { //negativo
          lessExp = f_exp > g_exp;
          lessFrac = f_frac > g_frac;
        }

      return lessExp || (f_exp == g_exp && lessFrac);
    }

    lessSing = (uf >> 31) > (ug >> 31);
    return lessSing;
}

/*
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {
  unsigned signo_uf;
  unsigned exp_uf;
  unsigned frac_uf;
  int infinito;
  int exp_invalido;
  int exponente;
  int resultado;
  int positivo;

  signo_uf = (uf >> 31) & 1;
  exp_uf = (uf >> 23) & 0xFF;
  frac_uf = uf & (0x7f << 16 | 0xFF << 8 | 0xFF);
  infinito = (0x80 << 24) | 0;
  exp_invalido = 0xFF;
  positivo = (signo_uf ? -1:1);

  if (exp_uf == exp_invalido){
    return infinito;
  }

  if (exp_uf == 0){
    return 0;
  }

  exponente = exp_uf - 127;
  resultado = 0;
  if (exponente > 23){
    return infinito;
  }

  if (exponente < 0){
    return 0;
  }

  if (exponente == 0){
    resultado = ((frac_uf | (1 << 23)) >> (23 - exponente)) * positivo;
  }
  if (exponente > 0){
    resultado = (frac_uf >> (23 - exponente)) * positivo;
  }
  return resultado;
}
/*
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 *
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatPower2(int x) {
  int minimo_exponente;
  int maximo_exponente; 
  int bias;
  minimo_exponente = 0x00;
  maximo_exponente = 0xFF; 
  bias = 127;
  if (x < minimo_exponente - bias){
    return 0;
  }

  if (x + bias > maximo_exponente){
    return (maximo_exponente << 23);
  }
  
  return ((x + bias) << 23);
}
