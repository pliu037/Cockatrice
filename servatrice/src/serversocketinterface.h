/***************************************************************************
 *   Copyright (C) 2008 by Max-Wilhelm Bruker   *
 *   brukie@laptop   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef SERVERSOCKETINTERFACE_H
#define SERVERSOCKETINTERFACE_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QMutex>
#include "server_protocolhandler.h"

class QTcpSocket;
class Servatrice;
class DeckList;
class ServerInfo_DeckStorage_Folder;

class ServerSocketInterface : public Server_ProtocolHandler
{
	Q_OBJECT
private slots:
	void readClient();
	void catchSocketError(QAbstractSocket::SocketError socketError);
	void flushOutputBuffer();
signals:
	void outputBufferChanged();
private:
	QMutex outputBufferMutex;
	Servatrice *servatrice;
	QTcpSocket *socket;
	
	QByteArray inputBuffer, outputBuffer;
	bool messageInProgress;
	int messageLength;
	
	Response::ResponseCode cmdAddToList(const Command_AddToList &cmd, ResponseContainer &rc);
	Response::ResponseCode cmdRemoveFromList(const Command_RemoveFromList &cmd, ResponseContainer &rc);
	int getDeckPathId(int basePathId, QStringList path);
	int getDeckPathId(const QString &path);
	bool deckListHelper(int folderId, ServerInfo_DeckStorage_Folder *folder);
	Response::ResponseCode cmdDeckList(const Command_DeckList &cmd, ResponseContainer &rc);
	Response::ResponseCode cmdDeckNewDir(const Command_DeckNewDir &cmd, ResponseContainer &rc);
	void deckDelDirHelper(int basePathId);
	Response::ResponseCode cmdDeckDelDir(const Command_DeckDelDir &cmd, ResponseContainer &rc);
	Response::ResponseCode cmdDeckDel(const Command_DeckDel &cmd, ResponseContainer &rc);
	Response::ResponseCode cmdDeckUpload(const Command_DeckUpload &cmd, ResponseContainer &rc);
	DeckList *getDeckFromDatabase(int deckId);
	Response::ResponseCode cmdDeckDownload(const Command_DeckDownload &cmd, ResponseContainer &rc);
	Response::ResponseCode cmdBanFromServer(const Command_BanFromServer &cmd, ResponseContainer &rc);
	Response::ResponseCode cmdShutdownServer(const Command_ShutdownServer &cmd, ResponseContainer &rc);
	Response::ResponseCode cmdUpdateServerMessage(const Command_UpdateServerMessage &cmd, ResponseContainer &rc);
public:
	ServerSocketInterface(Servatrice *_server, QTcpSocket *_socket, QObject *parent = 0);
	~ServerSocketInterface();
	QHostAddress getPeerAddress() const { return socket->peerAddress(); }
	QString getAddress() const { return socket->peerAddress().toString(); }

	void transmitProtocolItem(const ServerMessage &item);
};

#endif
