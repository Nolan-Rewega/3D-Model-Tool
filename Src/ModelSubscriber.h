#ifndef SUB_H
#define SUB_H

/* Interface for subscribers to get notified of model change. */
class ModelSubscriber {
public:
	virtual void modelChanged() = 0;
};


#endif // !SUB_H
