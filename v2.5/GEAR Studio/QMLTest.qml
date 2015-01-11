import QtQuick 2.0
import CustomLib 1.0

Rectangle {
  width:640; height:360;

  CustomCom {
    id:customcom;
  }

  Component.onCompleted: {
    customcom.test();
  }
}