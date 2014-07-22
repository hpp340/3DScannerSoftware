/* This class is a checkable action inherited from QAction
 * The main difference is that this action can send two kinds of different signals
 * if it's checked, it sends actionCheck(), or it sends actionUncheck()

 * Author: Jerome Jiang
 * Date: 7/22/2014
*/

#pragma once
#include <QAction>
class checkableAction :
	public QAction
{
	Q_OBJECT
public:
	explicit checkableAction(QObject *parent=0);
	~checkableAction();
signals:
	void actionCheck();
	void actionUncheck();

private slots:
	void forwardCheckSignal();
};

