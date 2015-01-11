#pragma once
#include <QQuickItem>

/** 
* @created	2015/01/10
* @author Andy Reimann <a.reimann@moorlands-grove.de>
*/
class QmlOverlay : public QQuickItem
{
	Q_OBJECT // must include this if you use Qt signals/slots

	public:
		QmlOverlay(QQuickItem* parent = 0);
	public slots:
		void test() 
		{
			qDebug("Test!");
		}
	private:
};

