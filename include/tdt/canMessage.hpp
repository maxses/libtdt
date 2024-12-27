#ifndef TDT_CAN_MESSAGE_HPP
#define TDT_CAN_MESSAGE_HPP
//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------


//---Includes-----------------------------------------------------------------


#include <stdint.h>


//---Declarations-------------------------------------------------------------


typedef uint32_t canId_t;

class CCanMessage
{
      canId_t  m_id;
      int      m_len;
      uint8_t  m_data[8];
      
   public:
      constexpr CCanMessage()
         :m_id{0}
         ,m_len{0}
      {
         //
      }
      canId_t getId() const
      {
         return( m_id );
      }
      void setId(canId_t id)
      {
         m_id = id;
      }
      int getLen() const
      {
         return( m_len );
      }
      void setLen( int len )
      {
         m_len = len;
      }
      const uint8_t* getData() const
      {
         return( m_data );
      }
      uint8_t* getData()
      {
         return( m_data );
      }
      void setData(int len, const uint8_t* data)
      {
         m_len= len > 8 ? 8 : len;
         memcpy(m_data, data, m_len);
      }
};


//---fin-----------------------------------------------------------------------
#endif // ? ! TDT_CAN_MESSAGE_HPP
