#pragma once

//#include "../qorm_const.h"
//#include "../qorm_object.h"
#include <QObject>

namespace QOrm {

//Qt::AlignLeft 0x0001 Aligns with the left edge.
//Qt::AlignRight 0x0002 Aligns with the right edge.
//Qt::AlignHCenter 0x0004 Centers horizontally in the available space.
//Qt::AlignJustify 0x0008 Justifies the text in the available space.

//!
//! \brief The DtoAlign enum
//!
enum DtoAlign{
    daStart = Qt::AlignLeft | Qt::AlignVCenter
  , daCenter = Qt::AlignHCenter | Qt::AlignVCenter
  , daEnd = Qt::AlignRight  | Qt::AlignVCenter
  , daJustify = Qt::AlignJustify | Qt::AlignVCenter
};


enum DtoCompatibleValues{
      dcNone=1
    , dcQS  =2/*QML Style Sheets*/
    , dcQSS =4/*Qt Style Sheets*/
    , dcCSS =8/*Cascading Style Sheets*/
};

//!
//! \brief The DtoFilterStyle enum
//!
enum DtoFilterStyle{
    dsAutoDectect=1
};

}
