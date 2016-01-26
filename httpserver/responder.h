/*
 * pavelkolodin@gmail.com
 *
 * Responder example.
 *
 */

#include <fir/structures/flat_buff.h>
#include <fir/net/connection_http.h>
#include <fir/net/connection_bin.h>

template <typename T_RESOURCES>
class Responder {

	T_RESOURCES *resources_;
	fir::structures::FlatBuff *flatbuff_;

public:
	Responder(T_RESOURCES *_r)
	: resources_(_r)
	{
		// flatbuff is auto-resizable flat buffer of bytes
		flatbuff_ = resources_->getFlatBuff();
	}

	~Responder() {
		resources_->freeFlatBuff(flatbuff_);
	}

	// Called before new request.
	void clear() {
		std::cout << "CLEAR Responder\n";
	}

	// Called by ConnectionHTTP every time server is ready to send HTTP response to user.
	// "server is ready to send" may happen in following cases:
	// 1. New HTTP request got from client.
	// 2. New data received by another connection created by this Responder during previous call of this method.
	//
	// RETURN:
	void data(fir::net::ConnectionHTTP<T_RESOURCES, Responder> &_conn)
	{
		auto &headers = _conn.request().headers();

		std::cout << "HTTP request, host " << headers.m_host << ", uri " << headers.m_uri << "\n";


		//
		// Prepare variables for templater:
		//
		// { "data" : [
		//                { "User Agent" : "<user's user agent>" },
		//				  { "uri" : "<URI>" },
		//            ] }
		//
		typedef fir::structures::Dmap Dmap;

		Dmap vars;
		auto &vec = vars["data"].make_vector().get_vector();

		Dmap v;
		v["name"] = "User Agent";
		v["value"] = headers.m_user_agent;
		vec.push_back(v);

		v["name"] = "URI";
		v["value"] = headers.m_uri;
		vec.push_back(v);

		v["name"] = "Host";
		v["value"] = headers.m_host;
		vec.push_back(v);
		//
		// Render HTML page
		//
		flatbuff_->clear();
		resources_->getTemplater().get_template("index.html").perform(*flatbuff_, vars);

		std::cout << "response size: " << flatbuff_->size() << "\n";

		//
		// set response
		//
		_conn.m_headers_out.m_response_code = 200;
		_conn.m_headers_out.m_content_type = fir::net::http::HttpHeaders::ContentType::text_html;
		_conn.set_body(flatbuff_->data(), flatbuff_->size());
	}


	// Called on WebSocket message arrived
	void data(fir::net::ConnectionWebSocket<Responder> &_conn)
	{
		// EMPTY
	}

	void data(fir::net::ConnectionBin<Responder> &_conn)
	{
		// EMPTY
	}

	// Called by ConnectionHTTP when HTTP upgrade happened (HTTP -> WeSocket)
	// When this method called:
	// 1. HTTP response "HTTP/1.1 101 Switching Protocols" JUST SENT to user (written to a socket)
	// 2. ConnectionWebSocket object is created
	// This method might be used for initialization websocket session.
	void upgrade(fir::net::ConnectionHTTP<T_RESOURCES, Responder> &_conn_http, fir::net::ConnectionWebSocket<Responder> &_conn_ws)
	{
		// EMPTY
	}


	void close(fir::net::ConnectionHTTP<T_RESOURCES, Responder> &_conn)
	{
		std::cout << "HTTP connection closed\n";
	}

	void close(fir::net::ConnectionBin<Responder> &_conn)
	{
		// EMPTY
	}

	void close(fir::net::ConnectionWebSocket<Responder> &_conn)
	{
		// EMPTY
	}
};
