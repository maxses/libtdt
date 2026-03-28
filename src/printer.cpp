/**---------------------------------------------------------------------------
 *
 * @file    printer.hpp
 * @brief   Generate text fragments from tdt messages
 *
 * Those text strings can be used by user interfaces on host PC.
 * For text fragments to be used on target microcontroller, see "printer_biwak.hpp".
 * 
 * @date      20241214
 * @author    Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <tdt/printer.hpp>


/*--- Implementation -------------------------------------------------------*/


namespace Tdt
{

QString CPrinter::printValue( EFunctionCode functionCode, EUnit unit, const SValue& value ) const
{
   QString s;
   QTextStream ts( &s );
   switch( functionCode )
   {
      case Tdt::EFunctionCode::alert:
         s="Alert:";
         break;
      default:
         break;
   }
      
   if( m_unitMap.contains( unit ) )
   {
      SUnitDesc unitDesc=m_unitMap[unit];
      if( ( unitDesc.format && ( unitDesc.format != ' ' ) ) || ( unitDesc.divider != 1 ) )
      {
         return( printValueGeneric( functionCode, unit, value, unitDesc ) );
      }
   }
   
   switch( unit )
   {
      case Tdt::EUnit::version:
      {
         Tdt::SValue::SSoftwareVersion sw
             =value.softwareVersion;
         ts       << ( sw.drift.bootloader ? "BL " : "")
            << "v"
            << sw.major << "." << sw.minor << "." << sw.patch
            << "-" << sw.drift.distance
            << ( sw.drift.dirty ? "+" : "");
         break;
      }
      case Tdt::EUnit::durationSeconds:
      {
         int secs = (int)value._int;
         if ( qAbs(secs) < 100 )
         {
            ts << value._int << " s";
         }
         else
         {
            if( secs<0 )
            {
               ts << "- ";
               secs=-secs;
            }
            int days = secs / SEC_PER_DAY;
            secs-=days * SEC_PER_DAY;
            int hours =  secs / SEC_PER_HOUR;
            secs-=hours * SEC_PER_HOUR;
            int minutes =  secs / SEC_PER_MIN;
            secs-=minutes * SEC_PER_MIN;
            
            if(days)
            {
               ts << days << "d ";
            }
            ts << hours << ":" << minutes << ":" << secs << "s";
         }
         break;
      }
      case Tdt::EUnit::time:
      {
         Tdt::SValue::STime t=value.time;
         ts << t.hour << ":" << t.min << ":" << t.sec;
         break;
      }
      case Tdt::EUnit::percentHumidity:
      {
         ts << ( ( value._int / 10 ) / 100.0) << " %";
         break;
      }
      case Tdt::EUnit::_switch:
      {
         ts << ( value._bool ? "ON" : "OFF" );
         break;
      }
      case Tdt::EUnit::logCode:
      {
         Tdt::ELog log=toLog( value.logCode );
         ELogCategory cat=Tdt::toCategory( value.logCode );
         
         // Inforunately lepto is not availabvle
         switch( (int)cat )
         {
            // Lepto files are not included
            case 0:
               // Debug/ Good/ Info
               ts << "Info: ";
               break;
            case 1:
               ts << "Caution: ";
               break;
            case 2:
               ts << "Warning: ";
               break;
            case 3:
               ts << "Critical: ";
               break;
            case 4:
               // Does not happen at all
               ts << "Fatal: ";
               break;
            default:
               ts << "Other: ";
               break;
         }
         
         if( m_logMap.contains( log ) )
         {
            ts << m_logMap[ log ];
         }
         else
         {
            ts.setIntegerBase(16);
            ts << "UK: 0x" << value._uint;
         }
         break;
      }
      case Tdt::EUnit::milliVolt:
      {
         ts << value._int << " mV";
         break;
      }
      default:
      {
         s=QString( "<No Format: Unit %1>" ).arg( (int)unit );
      }
   } // switch unit
   
   return(s);
};


QString CPrinter::printValueGeneric( EFunctionCode functionCode, EUnit unit,
               const SValue& value, const SUnitDesc& unitDesc ) const
{
   QString s;
   
   s+=QString::fromUtf8( unitDesc.prefix );
   
   switch( unitDesc.format )
   {
      case 'E':
      {
         if( ! unitDesc.enumValues.contains( value._int ) )
         {
            return( QString( "%1::%2" ).arg( unitDesc.name ).arg( value._int ) );
         }
         s += unitDesc.enumValues[ value._int ];
         break;
      }
      case 'x':
      {
         s += QString( "0x%1" ).arg((unsigned int)( value._uint / unitDesc.divider ), 0, 16);
         break;
      }
      case 'd':
      {
         s += QString( "%1" ).arg((unsigned int)( value._uint / unitDesc.divider ) );
         break;
      }
      default:
      {
         if( unitDesc.divider != 1 )
         {
            float t = value._int;
            t /= (   // If unitDesc.decimalPower is '0' assume '1'
                     unitDesc.divider ? unitDesc.divider : 1 );
            s += QString("%1").arg( t );
         }
         else
         {
            s += QString("<Unknown format: %1>").arg( unitDesc.format );
         }
         break;
      }
   } // switch format

   if( s.isEmpty() )
   {
      return("<??? generic>");
   }
   
   if( unitDesc.postfix[0] )
   {
      s += QString::fromUtf8( unitDesc.postfix );
   }
   
   return( s );
}


}; // namespace Tdt


/*--- Fin ------------------------------------------------------------------*/
