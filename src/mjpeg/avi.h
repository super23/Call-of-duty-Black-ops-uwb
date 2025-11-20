#pragma once

void __thiscall avi::RIFF_s::RIFF_s(avi::RIFF_s *this);
void __thiscall avi::RIFF_s::LIST_hdrl_s::LIST_hdrl_s(avi::RIFF_s::LIST_hdrl_s *this);
void __thiscall avi::RIFF_s::LIST_hdrl_s::avih_s::avih_s(avi::RIFF_s::LIST_hdrl_s::avih_s *this);
void __thiscall avi::RIFF_s::LIST_hdrl_s::LIST_vid_strl_s::LIST_vid_strl_s(
        avi::RIFF_s::LIST_hdrl_s::LIST_vid_strl_s *this);
void __thiscall avi::RIFF_s::LIST_hdrl_s::LIST_vid_strl_s::strh_s::strh_s(
        avi::RIFF_s::LIST_hdrl_s::LIST_vid_strl_s::strh_s *this);
void __thiscall avi::RIFF_s::LIST_hdrl_s::LIST_vid_strl_s::strf_s::strf_s(
        avi::RIFF_s::LIST_hdrl_s::LIST_vid_strl_s::strf_s *this);
void __thiscall avi::RIFF_s::LIST_hdrl_s::LIST_aud_strl_s::LIST_aud_strl_s(
        avi::RIFF_s::LIST_hdrl_s::LIST_aud_strl_s *this);
void __thiscall avi::RIFF_s::LIST_hdrl_s::LIST_aud_strl_s::strh_s::strh_s(
        avi::RIFF_s::LIST_hdrl_s::LIST_aud_strl_s::strh_s *this);
void __thiscall avi::RIFF_s::LIST_hdrl_s::LIST_aud_strl_s::strf_s::strf_s(
        avi::RIFF_s::LIST_hdrl_s::LIST_aud_strl_s::strf_s *this);
unsigned int __stdcall mjpeg_write_thread(void *__formal);
void __thiscall avi_s::write_thread(avi_s *this);
void __cdecl mjpeg_create(const char *szFile, int width, int height, unsigned int fps);
void __thiscall avi_s::create(
        avi_s *this,
        const char *szFile,
        unsigned int width,
        unsigned int height,
        unsigned int fps);
void __thiscall avi::RIFF_s::Out(avi::RIFF_s *this, unsigned __int8 **out);
unsigned int __cdecl avi::Out32(unsigned __int8 **out, unsigned int d);
void __thiscall avi::RIFF_s::LIST_hdrl_s::Out(avi::RIFF_s::LIST_hdrl_s *this, unsigned __int8 **out);
void __thiscall avi::RIFF_s::LIST_hdrl_s::avih_s::Out(avi::RIFF_s::LIST_hdrl_s::avih_s *this, unsigned __int8 **out);
void __thiscall avi::RIFF_s::LIST_hdrl_s::LIST_vid_strl_s::Out(
        avi::RIFF_s::LIST_hdrl_s::LIST_vid_strl_s *this,
        unsigned __int8 **out);
void __thiscall avi::RIFF_s::LIST_hdrl_s::LIST_vid_strl_s::strf_s::Out(
        avi::RIFF_s::LIST_hdrl_s::LIST_vid_strl_s::strf_s *this,
        unsigned __int8 **out);
void __thiscall avi::RIFF_s::LIST_hdrl_s::LIST_aud_strl_s::Out(
        avi::RIFF_s::LIST_hdrl_s::LIST_aud_strl_s *this,
        unsigned __int8 **out);
void __thiscall avi::RIFF_s::LIST_hdrl_s::LIST_vid_strl_s::strh_s::Out(
        avi::RIFF_s::LIST_hdrl_s::LIST_aud_strl_s::strh_s *this,
        unsigned __int8 **out);
void __thiscall avi::RIFF_s::LIST_hdrl_s::LIST_aud_strl_s::strf_s::Out(
        avi::RIFF_s::LIST_hdrl_s::LIST_aud_strl_s::strf_s *this,
        unsigned __int8 **out);
void __thiscall avi::RIFF_s::Setup(avi::RIFF_s *this, unsigned int width, unsigned int height, unsigned int fps);
void __cdecl mjpeg_add_frame(unsigned __int8 *y, unsigned __int8 *u, unsigned __int8 *v);
void __thiscall avi_s::add_frame(avi_s *this, unsigned __int8 *y, unsigned __int8 *u, unsigned __int8 *v);
void __thiscall avi::RIFF_s::AddFrame(avi::RIFF_s *this, unsigned int offset, unsigned int size);
unsigned int __thiscall avi_s::output_data(avi_s *this, unsigned __int8 *data, unsigned int size);
void __cdecl mjpeg_close_int();
void __thiscall avi_s::close(avi_s *this);
void __thiscall avi::idx1_s::Out(avi::idx1_s *this, unsigned __int8 **out);
void __thiscall avi_s::flush_samples(avi_s *this);
void __cdecl mjpeg_add_samples(__int16 *in, int count);
void __thiscall avi_s::add_samples(avi_s *this, __int16 *in, int count);
