#ifdef POWERMATE
#ifndef POWERMATE_H
#define POWERMATE_H

#include <QThread>
#include <QFile>
#include <QObject>

struct input_event;

class KnobEvent {
    protected:
        int m_delta;
        Qt::MouseButtons m_buttons;
        Qt::KeyboardModifiers m_modifiers;
    public:
        KnobEvent():
            m_delta(0),
            m_buttons(Qt::NoButton),
            m_modifiers(Qt::NoModifier){}

        KnobEvent(int delta, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers):
            m_delta(delta),
            m_buttons(buttons),
            m_modifiers(modifiers){}
        virtual ~KnobEvent(){}

        int delta() { return m_delta; }
        Qt::MouseButtons buttons() { return m_buttons; }
        Qt::KeyboardModifiers modifiers() { return m_modifiers; }
};

class PmInput : public QThread {
	Q_OBJECT
	public:
		PmInput(QObject *parent=0, const char *device=0);
		~PmInput();

	protected:
		void run();

	signals:
		void knobEvent(KnobEvent event);

	private:
		QString devicename;
		void processEvent(const input_event& event);
        Qt::MouseButtons m_buttons;
		//void setLed(const input_event& event);// set powermate led
};

#endif
#endif //POWERMATE
