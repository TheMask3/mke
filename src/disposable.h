/*
** disposable.h
**
** This file is part of mke.
**
** Copyright (C) 2013 Jonas Kulla <Nyocurio@gmail.com>
**
** mke is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 2 of the License, or
** (at your option) any later version.
**
** mke is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with mke.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DISPOSABLE_H
#define DISPOSABLE_H

#include "intrulist.h"
#include "exception.h"
#include "sharedstate.h"
#include "graphics.h"

#include <assert.h>
#include "sigslot/signal.hpp"

class Disposable
{
public:
	Disposable()
	    : disposed(false),
	      link(this)
	{
		shState->graphics().addDisposable(this);
	}

	virtual ~Disposable()
	{
		shState->graphics().remDisposable(this);
	}

	void dispose()
	{
		if (disposed)
			return;

		releaseResources();
		disposed = true;
		wasDisposed();
	}

	bool isDisposed() const
	{
		return disposed;
	}

    sigslot::signal<> wasDisposed;

protected:
	void guardDisposed() const
	{
		if (isDisposed())
			throw Exception(Exception::RGSSError,
		                    "disposed %s", klassName());
	}

private:
	virtual void releaseResources() = 0;
	virtual const char *klassName() const = 0;

	friend class Graphics;

	bool disposed;
	IntruListLink<Disposable> link;
};

template<class C>
inline bool
nullOrDisposed(const C *d)
{
	if (!d)
		return true;

	if (d->isDisposed())
		return true;

	return false;
}

#endif // DISPOSABLE_H
