#ifndef TDT_CAN_MESSAGE_HPP
#define TDT_CAN_MESSAGE_HPP
//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------


//---Includes-----------------------------------------------------------------


#include <cstring>      // memcpy
#include <stdint.h>


//---Declarations-------------------------------------------------------------


typedef uint32_t canId_t;

struct SCanMessage
{
   private:
      canId_t  m_id;
      int      m_len;
      uint8_t  m_data[8];
      
   public:
      constexpr SCanMessage( canId_t id )
          :m_id{ id }
          ,m_len{ 0 }
          ,m_data{ 0, 0, 0, 0,
                   0, 0, 0, 0 }
      {
      }

      #if defined CONFIG_TDT_CONSTEXPR_DEFAULT_CONSTRUCTOR
      constexpr SCanMessage()
         // Must be initialized in constexpr
         :m_id{0}
         ,m_len{0}
         ,m_data{ 0, 0, 0, 0,
                  0, 0, 0, 0 }
      {
      }
      #else
      SCanMessage()
      {};
      #endif

      constexpr SCanMessage( canId_t id, int len )
          :m_id{id}
          ,m_len{ len > 8 ? 8 : len }
          ,m_data{ 0, 0, 0, 0,
                   0, 0, 0, 0 }
      {
      }
      constexpr SCanMessage( canId_t id, int len, const uint8_t* data )
          :m_id{id}
          ,m_len{ len > 8 ? 8 : len }
          ,m_data{   data[0], data[1], data[2], data[3],
                     data[4], data[5], data[6], data[7] }
      {
         //
      }
      constexpr canId_t getId() const
      {
         return( m_id );
      }
      //constexpr does not work
      void setId(canId_t id)
      {
         m_id = id;
      }
      constexpr int getLen() const
      {
         return( m_len );
      }
      //constexpr does not work on gcc 10
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


static_assert( sizeof( SCanMessage ) == 16, "Wrong CAN message size" );


//---fin-----------------------------------------------------------------------
#endif // ? ! TDT_CAN_MESSAGE_HPP
