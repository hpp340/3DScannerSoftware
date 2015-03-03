#pragma once
#include <QAction>

/*! \brief This class is a checkable action inherited from QAction
* \details The main difference is that this action can send two kinds of different signals
* \details if it's checked, it sends actionCheck(), or it sends actionUncheck()

* \author Jerome Jiang
* \date 7/22/2014
*/

class checkableAction :
	public QAction
{
	Q_OBJECT
public:

	/// Constructor
	explicit checkableAction(QObject *parent=0);

	/// Deconstructor
	~checkableAction();
signals:

	/// If the button is checked already, then it sends this signal.
	void actionCheck();

	/// If the button is unchecked, then it sends this signal.
	void actionUncheck();

private slots:

	/// This Slot is used to emit signals. The rules are as the introduction.
	void forwardCheckSignal();
};

