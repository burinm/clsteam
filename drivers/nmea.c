#include "nmea.h"

uint8_t nmea_add_char_to_message(char c){
  uint8_t rtn;

  rtn=1;

  if(c=='$')
  {
    nmea_clear_message();
  }

  if(nmea_string_index<NMEA_MAX_LEN)
  {
    nmea_string[nmea_string_index]=c;
    nmea_string_index++;
  }
  else
  {
    nmea_clear_message();
    rtn=0;
  }

  return rtn;

}

uint8_t nmea_check_message_complete(){
  uint8_t rtn;

  if((nmea_string_index>0))
  {
    if(nmea_string[nmea_string_index-1]=='\n')
    {
      rtn = 1;
    }
    else
    {
      rtn = 0;
    }
  }
  else
  {
    rtn = 0;
  }

  return rtn;

}

void nmea_clear_message(){
  int iter;
  for(iter=nmea_string_index;iter>=0;iter--)
    nmea_string[iter]='\0';
  nmea_string_index=0;
}

uint8_t nmea_parse_message() {
  uint8_t rtn;

  rtn=1;

  if(nmea_str_split(NMEA_TYPE_INDEX))
  {
    //if(nmea_field_comp(GGA))
    if(strncmp(GGA, nmea_field, NMEA_MAX_LEN) == 0)
    {
      if(nmea_str_split(LATITUDE))
      {
        strcpy(nmea_gps_coords.latitude, nmea_field);
        //nmea_field_copy(nmea_gps_coords.latitude);

        if(nmea_str_split(LONGITUDE))
        {
          strcpy(nmea_gps_coords.longitude, nmea_field); 
          //nmea_field_copy(nmea_gps_coords.longitude);

          if(nmea_str_split(ALTITUDE))
          {
            strcpy(nmea_gps_coords.altitude, nmea_field);
            //nmea_field_copy(nmea_gps_coords.altitude);

            if(nmea_str_split(NS_INDICATOR))
            {
              strcpy(nmea_gps_coords.ns_indicator, nmea_field);
              //nmea_field_copy(nmea_gps_coords.ns_indicator);

              if(nmea_str_split(EW_INDICATOR))
              {
                strcpy(nmea_gps_coords.ew_indicator, nmea_field);
                //nmea_field_copy(nmea_gps_coords.ew_indicator);
              }
              else
              {
                rtn=0;
              }
            }
            else
            {
              rtn=0;
            }
          }
          else
          {
            rtn=0;
          }
        }
        else
        {
          rtn=0;
        }
      }
      else
      {
        rtn=0;
      }
    }
    else
    {
      rtn=0;
    }
  }
  else
  {
    rtn=0;
  }

  return rtn;

}

uint8_t nmea_str_split(unsigned int index){
  uint8_t rtn;
  unsigned int iter;
  unsigned int local_index;
  unsigned int field_index;

  rtn=1;
  local_index = 0;
  field_index = 0;

  for(iter=0;iter<NMEA_MAX_LEN&&local_index<=index;iter++)
  {
    if(nmea_string[iter]==',')
    {
      local_index++;
    }
    else if(local_index == index)
    {
      nmea_field[field_index]=nmea_string[iter];
      if(field_index < NMEA_FIELD_MAX_LEN)
      {
        field_index++;
      }
      else
      {
        local_index++;
      }
    }
  }

  nmea_field[field_index]='\0';

  if(field_index==0)rtn=0;

  return rtn;

}

uint8_t nmea_field_comp(const char *c){
  uint8_t rtn;
  unsigned int iter;

  rtn=1;

  for(iter=0;iter<NMEA_MAX_LEN&&c[iter]!='\0'&&rtn==1;iter++){
    if(c[iter]!='\0'){
      if(c[iter]!=nmea_field[iter])rtn=0;
    }
  }

  return rtn;

}

uint8_t nmea_field_copy(char dest[NMEA_FIELD_MAX_LEN]){
  uint8_t rtn;
  unsigned int iter;

  rtn=1;

  for(iter=0;iter<NMEA_MAX_LEN&&nmea_field[iter]!='\0'&&rtn==1;iter++){
    dest[iter]=nmea_field[iter];
  }

  return rtn;

}

uint8_t nmea_field_copy_short(char dest[NMEA_FIELD_IND_LEN]){
  uint8_t rtn;
  unsigned int iter;

  rtn=1;

  for(iter=0;iter<NMEA_FIELD_IND_LEN&&nmea_field[iter]!='\0'&&rtn==1;iter++){
    dest[iter]=nmea_field[iter];
  }

  return rtn;

}
