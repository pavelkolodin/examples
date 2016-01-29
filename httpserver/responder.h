/*
 * pavelkolodin@gmail.com
 *
 * Responder example.
 *
 */

#include <fir/structures/flat_buff.h>
#include <fir/net/responder_base.h>

template <typename T_RESOURCES>
class Responder : public fir::net::ResponderBase<T_RESOURCES, Responder<T_RESOURCES> >
{
	typedef fir::net::ResponderBase<T_RESOURCES, Responder<T_RESOURCES> > base_class;
	fir::structures::FlatBuff *flatbuff_;

	enum class State {
		START,
		WAITING
	};
	State state_ = State::START;

public:
	Responder(T_RESOURCES *_r)
	: base_class(_r)
	{
		// flatbuff is auto-resizable flat buffer of bytes
		flatbuff_ = base_class::resources_->getFlatBuff();
	}

	~Responder() {
		base_class::resources_->freeFlatBuff(flatbuff_);
	}

	// Called before new request.
	void clear() {
		std::cout << "CLEAR Responder\n";
	}

	void dataHTTP(fir::net::ConnectionHTTP<T_RESOURCES, Responder> &_conn)
	{
		auto &headers = _conn.request().headers();

		std::cout << "HTTP data ("<< _conn.m_id <<"), host " << headers.m_host << ", uri " << headers.m_uri << ", conn " << (void*)&_conn << "\n";

		if (headers.m_uri.startwith(BUFFPTR_SZ("/test1")) ) {
			path_test1(_conn);
			return;
		}
		//
		// Prepare variables for templater:
		// Dmap vars will carry this structure:
		//
		// { "data" : [
		//                { 	"name"  : "User Agent",
		//						"value" : "<user's user agent>" },
		//                {     "name"  : "uri",
		// 	                    "value" : "<HTTP URI>" },
		//                {     "name"  : "Host",
		// 	                    "value" : "<Host>" },
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
		base_class::resources_->getTemplater().get_template("index.html").perform(*flatbuff_, vars);

		std::cout << "response size: " << flatbuff_->size() << "\n";

		//
		// set response
		//
		_conn.get_headers_out().m_response_code = 200;
		_conn.get_headers_out().m_content_type = fir::net::http::HttpHeaders::ContentType::text_html;
		_conn.set_body(flatbuff_->data(), flatbuff_->size());
	}


	void path_test1(fir::net::ConnectionHTTP<T_RESOURCES, Responder> &_conn)
	{
		LOG_PURE("state " << (int)state_);

		switch (state_)
		{
		default:
		case State::WAITING:
			break;

		case State::START:
		{
			state_ = State::WAITING;
			auto conn = base_class::newConnectionHTTP(this);
			conn->get_headers_out().m_host = BUFFPTR_SZ("www.ya.ru");
			conn->get_headers_out().m_uri = BUFFPTR_SZ("/test");
			conn->get_headers_out().m_method = fir::net::http::HttpHeaders::Method::GET;
			conn->get_headers_out().m_version = fir::net::http::HttpHeaders::Version::HTTP_1_1;

			fir::net::Addr addr(IPADDR_TO_UINT32(213,180,193,3), 80);
			conn->m_remote_addr = addr;
		}
		break;

		}



//		typename T_RESOURCES::connection_http *conn = m_resources->newConnectionHTTP(this);
//		conn->m_remote_addr = a;
	}
};
