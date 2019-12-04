// Author Uzair Khan
// A Compiller buffer 

#include "buffer.h"

Buffer *b_alloc(short init_capacity,char inc_factor, char o_mode){
    Buffer *buffer = (Buffer *) calloc(1,sizeof(Buffer)); //Using calloc function so, all data initialize by 0 or NULL by default
    
    if(init_capacity < 0 && init_capacity > (SHRT_MAX-1)){
        return NULL;
    }

    if(init_capacity != 0){
        buffer->cb_head = (char *) malloc(init_capacity); //make the location in consective memmory heap with length of capacity
        buffer->capacity = init_capacity;
    }
    else{
        buffer->cb_head = (char *) malloc(DEFAULT_INIT_CAPACITY);
        buffer->capacity = DEFAULT_INIT_CAPACITY;

        if(o_mode == 'a' || o_mode == 'm'){
            buffer->inc_factor = DEFAULT_INC_FACTOR;
        }
        else if(o_mode == 'f'){
            buffer->inc_factor = 0;
        }
        else
        {
            return NULL;
        }
    }

    if(o_mode == 'f'){
        buffer->inc_factor = 0;
        buffer->mode = 0;
    }
    else if(inc_factor == 0 && init_capacity != 0){
        buffer->inc_factor = 0;
        buffer->mode = 0;
    }
    else if(o_mode == 'a' && inc_factor > 0 && inc_factor <=255){
        buffer->mode = 1;
        buffer->inc_factor = inc_factor;
    }
    else if(o_mode == 'm' && inc_factor > 0 && inc_factor <=100){
        buffer->mode = -1;
        buffer->inc_factor = inc_factor;
    }

    buffer->flags = DEFAULT_FLAGS;
    
    return buffer;
}

int b_isfull(Buffer * const pBD){
    if(pBD->addc_offset == pBD->capacity){ // if char buffer is full it is equal to offset and capacity
        return 1;
    }
    else{
        return 0;
    }
}

pBuffer b_addc(pBuffer const pBD,char symbol){
    short newCapacity = 0; // making some kind of global variable
    if((pBD->flags & CHECK_R_FLAG) == 1){
	 pBD->flags = pBD->flags & RESET_R_FLAG;
  }
  
  if(!b_isfull(pBD)){ // if it is not full, simply add the char symbol
      *(pBD->cb_head + pBD->addc_offset) = symbol;
      pBD->addc_offset++;
      return pBD;
  }
  
  if(pBD->mode == 0){
      return NULL;
  }
  else if(pBD->mode == 1){
      short res = ((pBD->inc_factor | 0x00) + pBD->capacity);
      if( res < 0){
          return NULL;
      }
      else if(res > (SHRT_MAX-1)){
          newCapacity = SHRT_MAX;
      }
      else if(res < (SHRT_MAX-1))
      {
          newCapacity = res;
      }
  }
  else if(pBD->mode == -1)
  {
      if(pBD->capacity >= (SHRT_MAX-1)){
          return NULL;
      }

      
      int availableSpace = (SHRT_MAX-1) - pBD->capacity;
      int incFactor = pBD->inc_factor;
      double newIncrement = availableSpace * (incFactor/100.0);
      newCapacity = pBD->capacity + newIncrement;

      if(newCapacity > (SHRT_MAX-1)){
          newCapacity = SHRT_MAX;
      }
  }

  
   char* ptr=(char *) realloc(pBD->cb_head,newCapacity);

   pBD->capacity = newCapacity;

   if (ptr == NULL){
       return NULL;
   }
   else
   {
       pBD->cb_head = ptr;
   }   
   
   pBD->flags = pBD->flags | SET_R_FLAG;

   *(pBD->cb_head + pBD->addc_offset) = symbol;
    pBD->addc_offset++;

    return pBD;
}

Buffer * b_compact(Buffer * const pBD, char symbol){
    short inc = ((pBD->addc_offset|0x00)+1);

    
    if(inc > (SHRT_MAX-1)){
        return NULL;
    }
     pBD->cb_head= (char *) realloc(pBD->cb_head, inc);
     *(pBD->cb_head + pBD->addc_offset) = symbol;
     pBD->addc_offset += 1;
     pBD->capacity = inc;
     pBD->flags = pBD->flags | SET_R_FLAG;

     return pBD;
}

int b_load (FILE * const fi, Buffer * const pBD){
    while(!feof(fi)){
        char ch = fgetc(fi);
        if(b_addc(pBD,ch) == NULL){
            ungetc(ch,fi);
            return LOAD_FAIL;
        }
    }

    return 1;
}

int b_eob (Buffer * const pBD){
    return (pBD->flags & CHECK_EOB);
}

int b_clear (Buffer * const pBD){
    if(pBD == NULL){
        return RT_FAIL_1;
    }
    pBD->addc_offset = 0;
    pBD->getc_offset = 0;
    pBD->markc_offset = 0;

    return 0;
}

void b_free (Buffer * const pBD){
    if(pBD != NULL){
        if(pBD->cb_head != NULL){
            free(pBD->cb_head);
        }

        free(pBD);
    }
}

short b_limit (Buffer * const pBD){
    if(pBD == NULL){
        return RT_FAIL_1;
    }
    return pBD->addc_offset;
}

short b_capacity(Buffer * const pBD){
    if(pBD == NULL){
        return RT_FAIL_1;
    }
    return pBD->capacity;
}

short b_mark (pBuffer const pBD, short mark){
    if(pBD == NULL){
        return RT_FAIL_1;
    }
    if(mark < 0 && mark > pBD->addc_offset){
        return RT_FAIL_1;
    }

    pBD->markc_offset = mark;

    return mark;
}

int b_mode (Buffer * const pBD){
    if(pBD == NULL){
        return RT_FAIL_2;
    }
    return (int)pBD->mode;
}

size_t b_incfactor (Buffer * const pBD){
    if(pBD == NULL){
        return 0x100;
    }

    return (pBD->inc_factor & 0xFF); /* Converting increment factor from -ve to +ve(actual value)*/
}

int b_isempty (Buffer * const pBD){
    if(pBD == NULL){
        return RT_FAIL_1;
    }
    if(pBD->addc_offset == 0){
        return 1;
    }
    return 0;
}

char b_getc (Buffer * const pBD){
    if(pBD == NULL){
        return RT_FAIL_2;
    }

    if(pBD->getc_offset > pBD->addc_offset){
        return RT_FAIL_2;
    }

    if(pBD->addc_offset == pBD->getc_offset){
        pBD->flags = (pBD->flags | SET_EOB);
        return 0;
    }

    char ch = *(pBD->cb_head + pBD->getc_offset);

    pBD->getc_offset++;

    return ch;
}

int b_print (Buffer * const pBD, char nl){
    if(pBD == NULL || pBD->cb_head == NULL){
        return RT_FAIL_1;
    }
    while(!b_eob(pBD)){
        char ch = b_getc(pBD);
        if(ch == -2){
            return RT_FAIL_1;
        }
        printf("%c",ch);
    }

    if(nl != 0){
        printf("\n");
    }

    return 1;
}

char b_rflag (Buffer * const pBD){
    if(pBD == NULL){
        return RT_FAIL_1;
    }
    return pBD->flags & CHECK_R_FLAG;
}

short b_retract (Buffer * const pBD){
    if(pBD == NULL || pBD->getc_offset <= 0){
        return RT_FAIL_1;
    }

    return (pBD->getc_offset--);
}

short b_reset (Buffer * const pBD){
    if(pBD == NULL){
        return RT_FAIL_1;
    }
    pBD->getc_offset = pBD->markc_offset;
    return pBD->getc_offset;
}

short b_getcoffset (Buffer * const pBD){
    if(pBD == NULL){
        return RT_FAIL_1;
    }

    return pBD->getc_offset;
}

int b_rewind(Buffer * const pBD){
    if(pBD == NULL){
        return RT_FAIL_1;
    }

    pBD->getc_offset = 0;
    pBD->markc_offset = 0;

    return 0;
}

char * b_location(Buffer * const pBD){
    if(pBD == NULL || pBD->cb_head == NULL){
        return NULL;
    }

    if(pBD->markc_offset > pBD->addc_offset){
        return NULL;
    }
    return (pBD->cb_head + pBD->markc_offset);
}
