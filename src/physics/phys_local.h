#pragma once
#include <universal/assertive.h>
#include <cstring>

#include <Windows.h> // interlockedxchg
#include <cmath>

struct phys_vec2 // sizeof=0x8
{                                                                             // XREF: contact_manifold_mesh_point/r
                                                                                // phys_contact_manifold_process::bridge/r ...
    float x;                                                        // XREF: phys_contact_manifold::generate_convex_poly_internal(void)+59/w
    // phys_contact_manifold::generate_convex_poly_internal(void)+70/w ...
    float y;                                                        // XREF: phys_contact_manifold::generate_convex_poly_internal(void)+68/w
    // phys_contact_manifold::generate_convex_poly_internal(void)+7C/w ...
};

struct phys_vec3 // sizeof=0x10
{                                                                             // XREF: .data:PHYS_X_VEC/r
    float x;                                                        // XREF: gjkcc_info::update_cg(float const * const,float const * const,bool)+1F2/r
    float y;                                                        // XREF: gjkcc_info::update_cg(float const * const,float const * const,bool)+209/r
    float z;                                                        // XREF: gjkcc_info::update_cg(float const * const,float const * const,bool)+221/r
    float w;                                                        // XREF: standard_query::query(broad_phase_environment_query_input const &,broad_phase_environement_query_results *)+440/r

    phys_vec3()
    {

    }
    phys_vec3(float setall)
    {
        x = setall;
        y = setall;
        z = setall;
        w = setall;
    }
    phys_vec3(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
        w = 0.0f;
    }
    phys_vec3(float _x, float _y, float _z, float _w)
    {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }

    inline float max_component() const // trash for the ai
    {
        float m = x > y ? x : y;
        return m > z ? m : z;
    }

    inline phys_vec3 &operator*=(float d)
    {
        x *= d;
        y *= d;
        z *= d;
        return *this;
    }

    inline phys_vec3 &operator+=(const phys_vec3 &v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    inline phys_vec3 &operator-=(const phys_vec3 &v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }


    inline phys_vec3 &operator-=(const phys_vec3 *v)
    {
        x -= v->x;
        y -= v->y;
        z -= v->z;
        return *this;
    }

    inline phys_vec3 &operator/=(float d)
    {
        float inv = 1.0f / d;
        x *= inv;
        y *= inv;
        z *= inv;
        return *this;
    }

    inline phys_vec3 &operator=(const phys_vec3 &v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }

    inline float &operator[](unsigned int i)
    {
        iassert(i < 3);
        return reinterpret_cast<float *>(this)[i];
    }

    inline const float &operator[](unsigned int i) const
    {
        iassert(i < 3);
        return reinterpret_cast<const float *>(this)[i];
    }

    inline bool operator<(const phys_vec3 &v) const
    {
        return x < v.x && y < v.y && z < v.z;
    }

    inline bool operator>(const phys_vec3 &v) const
    {
        return x > v.x && y > v.y && z > v.z;
    }

    inline bool operator<=(const phys_vec3 &v) const
    {
        return x <= v.x && y <= v.y && z <= v.z;
    }

    inline bool operator>=(const phys_vec3 &v) const
    {
        return x >= v.x && y >= v.y && z >= v.z;
    }

    inline phys_vec3 operator*(const float mul)
    {
        phys_vec3 r;
        r.x = x * mul;
        r.y = y * mul;
        r.z = z * mul;
        return r;
    }

    inline phys_vec3 operator+(const phys_vec3 &other)
    {
        phys_vec3 r;
        r.x = x + other.x;
        r.y = y + other.y;
        r.z = z + other.z;
        r.w = 0.0f;
        return r;
    }

    // negate
    inline phys_vec3 operator-() const
    {
        phys_vec3 r;
        r.x = -x;
        r.y = -y;
        r.z = -z;
        r.w = w;
        return r;
    }
};

// KISAKTODO: operator cleanup
inline phys_vec3 operator+(const phys_vec3 &a, const phys_vec3 &b)
{
    return { a.x + b.x, a.y + b.y, a.z + b.z, 0.0f };
}

inline phys_vec3 operator-(const phys_vec3 &a, const phys_vec3 &b)
{
    return { a.x - b.x, a.y - b.y, a.z - b.z, 0.0f };
}

inline phys_vec3 operator*(const phys_vec3 &a, float d)
{
    return { a.x * d, a.y * d, a.z * d, 0.0f };
}

inline phys_vec3 operator/(const phys_vec3 &a, float d)
{
    float inv = 1.0f / d;
    return { a.x * inv, a.y * inv, a.z * inv, 0.0f };
}

struct bpei_database_id // sizeof=0x8
{                                       // XREF: broad_phase_environment_info/r
    unsigned int m_id1;                 // XREF: gjk_physics_collision_visitor::query_create_prolog(void const *)+5F/w
    unsigned int m_id2;                 // XREF: gjk_physics_collision_visitor::query_create_prolog(void const *)+65/w

    bpei_database_id()
    {
    }

    bpei_database_id(unsigned int id) // this is psycho, but it looks like what's going on
    {
        m_id1 = id;
        m_id2 = 0;
    }
};

struct __declspec(align(16)) plane_lt // sizeof=0x20
{                                       // XREF: ?calc_winding@@YAXABV?$phys_static_array@Uplane_lt@@$0CAA@@@HAAV?$phys_static_array@Vphys_vec3@@$0CAA@@@@Z/r
    phys_vec3 n;
    float d;
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
};

struct chull_t // sizeof=0x20
{
    unsigned int key;
    int nverts;
    phys_vec3 *verts;
    int ninds;
    unsigned __int16 *inds;
    chull_t *next;
    int touched;
    chull_t *next_list;
};

struct minspec_mutex // sizeof=0x4
{                                       // XREF: .data:minspec_mutex g_render_mutex/r
    volatile unsigned int m_token;      // XREF: _dynamic_initializer_for__g_render_mutex__+3/w

    inline void Lock()
    {
        volatile unsigned int Target; // [esp+4h] [ebp-4h] BYREF

        while (_InterlockedCompareExchange(&this->m_token, 1, 0))
            ;
        Target = 0;
        InterlockedExchange(&Target, 0);
    }

    inline void Unlock()
    {
        LONG Target; // [esp+4h] [ebp-8h] BYREF

        Target = 0;
        InterlockedExchange(&Target, 0);

        iassert(this->m_token == 1);
        //if (this->m_token != 1
        //    && _tlAssert(
        //        "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mutex.h",
        //        85,
        //        "GetStuff32(&m_token) == 1",
        //        ""))
        //{
        //    __debugbreak();
        //}
        if (_InterlockedCompareExchange(&this->m_token, 0, 1) != 1)
        {
            iassert(0);
            //if (_tlAssert("c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mutex.h", 88, "retv", ""))
            //    __debugbreak();
        }
    }
};

struct minspec_read_write_mutex // sizeof=0x4
{                                       // XREF: phys_transient_allocator/r
    volatile unsigned int m_count;      // XREF: physics_system::time_step(float,bool)+158/w

    inline void ReadLock()
    {
        LONG Target[3]; // [esp+4h] [ebp-10h] BYREF
        unsigned int count; // [esp+10h] [ebp-4h]

        do
        {
            do
                count = this->m_count;
            while (!count);
            Target[1] = count;
            Target[2] = count + 1;
        } while (_InterlockedCompareExchange(&this->m_count, count + 1, count) != count);
        Target[0] = 0;
        InterlockedExchange(Target, 0);
    }

    void ReadUnlock()
    {
        volatile unsigned int count; // [esp+Ch] [ebp-4h]

        do
        {
            count = this->m_count;
            iassert(count > 1);
            //if (this->m_count <= 1)
            //{
            //    if (_tlAssert(
            //        "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mutex.h",
            //        116,
            //        "count > 1",
            //        ""))
            //    {
            //        __debugbreak();
            //    }
            //}
        } while (_InterlockedCompareExchange(&this->m_count, count - 1, count) != count);
    }

    inline void WriteLock()
    {
        LONG Target; // [esp+4h] [ebp-4h] BYREF

        while (_InterlockedCompareExchange(&this->m_count, 0, 1) != 1)
            ;
        Target = 0;
        InterlockedExchange(&Target, 0);
    }

    void WriteUnlock()
    {
        LONG Target; // [esp+4h] [ebp-8h] BYREF

        Target = 0;
        InterlockedExchange(&Target, 0);
        iassert(!this->m_count);
        //if (this->m_count
        //    && _tlAssert(
        //        "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mutex.h",
        //        135,
        //        "GetStuff32(&m_count) == 0",
        //        ""))
        //{
        //    __debugbreak();
        //}
        if (_InterlockedCompareExchange(&this->m_count, 1, 0) != 0)
        {
            //if (_tlAssert("c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mutex.h", 138, "retv", ""))
            //    __debugbreak();
        }
    }
};

struct phys_mat44 // sizeof=0x40
{                                                                             // XREF: .data:PHYS_IDENTITY_MATRIX/r
    phys_mat44(
        const phys_vec3 *x_,
        const phys_vec3 *y_,
        const phys_vec3 *z_,
        const phys_vec3 *w_)
    {
        this->x = *x_;
        this->y = *y_;
        this->z = *z_;
        this->w = *w_;
    }

    phys_mat44()
    {

    }

    void fix_w_column()
    {
        this->x.w = 0.0f;
        this->y.w = 0.0f;
        this->z.w = 0.0f;
        this->w.w = 1.0f;
    }

    phys_mat44& operator=(const phys_mat44 *that)
    {
            this->x.x = that->x.x;
            this->x.y = that->x.y;
            this->x.z = that->x.z;
            this->y.x = that->y.x;
            this->y.y = that->y.y;
            this->y.z = that->y.z;
            this->z.x = that->z.x;
            this->z.y = that->z.y;
            this->z.z = that->z.z;
            this->w.x = that->w.x;
            this->w.y = that->w.y;
            this->w.z = that->w.z;
            return *this;
    }
                                                                            // .data:PHYS_IDENTITY_MATRIX_0/r ...
    phys_vec3 x;                                                // XREF: gjk_cylinder_t::get_feature(phys_contact_manifold *)+63/r
    // gjk_cylinder_t::get_feature(phys_contact_manifold *)+70/r ...
    phys_vec3 y;                                                // XREF: gjk_cylinder_t::get_feature(phys_contact_manifold *)+95/o
    // SetIdentity(phys_mat44 &)+20/r ...
    phys_vec3 z;                                                // XREF: gjk_cylinder_t::get_feature(phys_contact_manifold *):loc_834A44/o
    // SetIdentity(phys_mat44 &)+3B/r ...
    phys_vec3 w; // position                                                // XREF: phys_calc_world_aabb(phys_vec3 const &,phys_vec3 const &,phys_mat44 const &,phys_vec3 *,phys_vec3 *)+10/w
    // create_obb_gjk_geom(float const (* const)[3],float const * const,float const * const,int,gjk_collision_visitor *)+10/w ...
};

template <typename T>
struct phys_link_list_base//<pulse_sum_node> // sizeof=0x4
{                                                                             // XREF: pulse_sum_node/r
        //pulse_sum_node *m_next_link;
        T *m_next_link;
};

template <typename T>
struct phys_link_list //<pulse_sum_node> // sizeof=0xC
{                                       // XREF: pulse_sum_constraint_solver/r
                                        // list_pulse_sum_node/r
    //pulse_sum_node *m_first;
    //pulse_sum_node **m_last_next_ptr;
    T *m_first;
    T **m_last_next_ptr;
    volatile unsigned int m_alloc_count;

    void add(T *p)
    {
        //if (!this->m_last_next_ptr
        //    && _tlAssert(
        //        "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
        //        230,
        //        "m_last_next_ptr",
        //        ""))
        //{
        //    __debugbreak();
        //}

        p->m_next_link = NULL;
        this->m_alloc_count++;
        *this->m_last_next_ptr = p;
        this->m_last_next_ptr = &p->m_next_link;
    }

    void add_mt(T *p)
    {
        p->m_next_link = nullptr;

        InterlockedIncrement(reinterpret_cast<volatile LONG *>(&m_alloc_count));

        T **prev = (T **)InterlockedExchangePointer(
            (PVOID *)&m_last_next_ptr,
            &p->m_next_link
        );

        *prev = p;
    }
};

template <typename T>
struct phys_link_list1 //<PhysObjUserData> // sizeof=0xC
{                                       // XREF: PhysGlob/r
    //PhysObjUserData *m_first;
    //PhysObjUserData *m_last;
    T *m_first;
    T *m_last;
    int m_alloc_count;                  // XREF: Phys_CollisionCallback(void)+1C/r

    void add(T *p)
    {
        T *i; // [esp+Ch] [ebp-4h]

        for (i = this->m_first; i; i = i->m_next_link)
        {
            iassert(i != p);
            //if (i == p)
            //{
            //    if (_tlAssert("c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_local.h", 135, "i != p", ""))
            //        __debugbreak();
            //}
        }
        ++this->m_alloc_count;
        if (this->m_last)
            this->m_last->m_next_link = p;
        else
            this->m_first = p;
        this->m_last = p;
        this->m_last->m_next_link = 0;
    }

    void remove(T *p)
    {
        T *i; // [esp+10h] [ebp-8h]
        T *last_i; // [esp+14h] [ebp-4h]

        i = this->m_first;
        last_i = 0;
        while (i)
        {
            if (p == i)
            {
                --this->m_alloc_count;
                if (last_i)
                    last_i->m_next_link = i->m_next_link;
                else
                    this->m_first = i->m_next_link;
                if (i == this->m_last)
                {
                    this->m_last = last_i;
                    iassert(!last_i || last_i->get_next_link() == NULL);
                    //if (last_i)
                    //{
                    //    if (last_i->m_next_link)
                    //    {
                    //        if (_tlAssert(
                    //            "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_local.h",
                    //            160,
                    //            "!last_i || last_i->get_next_link() == NULL",
                    //            ""))
                    //        {
                    //            __debugbreak();
                    //        }
                    //    }
                    //}
                }
                return;
            }
            last_i = i;
            i = i->m_next_link;
        }
        iassert(0);
        //if (_tlAssert("c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_local.h", 165, "0", ""))
        //    __debugbreak();
    }
};

template <typename T>
struct phys_simple_link_list//<contact_point_info> // sizeof=0x4
{                                                                             // XREF: rigid_body_constraint_contact/r
                                                                                // rigid_body_constraint_contact/r
        //contact_point_info *m_first;
        T *m_first;

        struct iterator//phys_simple_link_list<contact_point_info>::iterator // sizeof=0x4
        {                                       // XREF: ?render_contact@@YAXPAVrigid_body_constraint_contact@@@Z/r
            //contact_point_info *m_ptr;
            T *m_ptr;
        };
};

template <typename T, int TABLE_SIZE>
struct minspec_hash_table//<phys_slot_pool,64> // sizeof=0x10C
{                                       // XREF: phys_memory_manager/r
    //phys_slot_pool *m_hash_table[64];
    T *m_hash_table[TABLE_SIZE];
    unsigned int m_mod;
    unsigned int m_highest_collision;
    unsigned int m_total_collisions;

    // this got totally inlined
    inline T *find(unsigned int key)
    {
        T *entry = this->m_hash_table[key % this->m_mod];

        if (entry)
        {
            while (entry->m_map_key != key)
            {
                entry = entry->m_hash_next;
                if (!entry)
                    return nullptr;
            }

            return entry;
        }

        return nullptr;
    }

    inline void add(unsigned int key, T *entry_to_add)
    {
        T *entry; // edx
        unsigned int v5; // ebx
        unsigned int i; // ecx
        T *j; // eax
        unsigned int v8; // edx
        unsigned int v9; // eax
        unsigned int v10; // edi
        unsigned int v11; // ecx
        unsigned int v12; // edx
        unsigned int v13; // eax
        unsigned int m_mod; // eax
        unsigned int k; // edi
        T *v16; // ecx
        unsigned int v17; // edx
        unsigned int collision_counts[64]; // [esp+Ch] [ebp-200h] BYREF
        T *entry_list[64]; // [esp+10Ch] [ebp-100h]
        unsigned int total_collisions; // [esp+214h] [ebp+8h]
        unsigned int mod_i; // [esp+218h] [ebp+Ch]

        //entry = this->m_hash_table[key % this->m_mod];
        //
        //if (entry)
        //{
        //    while (entry->m_map_key != key)
        //    {
        //        entry = entry->m_hash_next;
        //        if (!entry)
        //            goto LABEL_7;
        //    }
        //    //if (_tlAssert(
        //    //    "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_hash_table.h",
        //    //    38,
        //    //    "find(key) == NULL",
        //    //    ""))
        //    //{
        //    //    __debugbreak();
        //    //}
        //}

        iassert(find(key) == NULL);

    LABEL_7:
        entry_list[0] = entry_to_add;
        v5 = 1;
        for (i = 0; i < TABLE_SIZE; ++i)
        {
            for (j = this->m_hash_table[i]; j; ++v5)
            {
                entry_list[v5] = j;
                j = j->m_hash_next;
            }
            this->m_hash_table[i] = 0;
        }
        this->m_mod = v5;
        this->m_highest_collision = 100000;
        this->m_total_collisions = 100000;
        v8 = v5;
        for (mod_i = v5; v8 < TABLE_SIZE; mod_i = v8)
        {
            if (v8)
                memset(collision_counts, 0, 4 * v8);
            v9 = 0;
            v10 = 0;
            v11 = 0;
            total_collisions = 0;
            if (v5)
            {
                do
                {
                    v12 = entry_list[v11]->m_map_key % mod_i;
                    v13 = ++collision_counts[v12];
                    if (v13 > v10)
                        v10 = collision_counts[v12];
                    total_collisions += v13;
                    ++v11;
                } while (v11 < v5);
                v8 = mod_i;
                v9 = total_collisions;
            }
            if (v9 < this->m_total_collisions)
            {
                this->m_mod = v8;
                this->m_highest_collision = v10;
                this->m_total_collisions = v9;
            }
            ++v8;
        }
        m_mod = this->m_mod;
        //if ((!m_mod || m_mod >= 0x40)
        //    && _tlAssert(
        //        "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_hash_table.h",
        //        83,
        //        "m_mod > 0 && m_mod < TABLE_SIZE",
        //        ""))
        //{
        //    __debugbreak();
        //}
        iassert(m_mod > 0 && m_mod < TABLE_SIZE);

        for (k = 0; k < v5; this->m_hash_table[v17] = v16)
        {
            v16 = entry_list[k];
            v17 = v16->m_map_key % this->m_mod;
            ++k;
            v16->m_hash_next = this->m_hash_table[v17];
        }
    }
};

char *__cdecl PMM_ALLOC(unsigned int size, unsigned int alignment);
void __cdecl PMM_VALIDATE(char *ptr, unsigned int size, unsigned int alignment);
void __cdecl PMM_FREE(unsigned __int8 *ptr, unsigned int size, unsigned int alignment);

template <typename T>
struct phys_simple_allocator//<phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal> // sizeof=0x4
{                                                                             // XREF: phys_heap_gjk_cache_system_avl_tree/r
        int m_count;

        T *allocate()
        {
                char *slot; // [esp+18h] [ebp-4h]

                slot = PMM_ALLOC(sizeof(T), sizeof(T) % 16 == 0 ? 16 : 4);
                if (!slot)
                        return 0;
                ++this->m_count;
                new ((void *)slot) T();
                return (T*)slot;
        }

        void free(T *slot)
        {
                if (slot)
                {
                        PMM_VALIDATE((char *)slot, sizeof(T), sizeof(T) % 16 == 0 ? 16 : 4);
                        --this->m_count;
                        slot->~T(); // disgusting
                        PMM_FREE((unsigned __int8 *)slot, sizeof(T), sizeof(T) % 16 == 0 ? 16 : 4);
                }
        }
};

//gjk_aabb_t *__thiscall phys_simple_allocator<gjk_aabb_t>::allocate(phys_simple_allocator<gjk_aabb_t> *this);
//gjk_obb_t *__thiscall phys_simple_allocator<gjk_obb_t>::allocate(phys_simple_allocator<gjk_obb_t> *this);
//gjk_brush_t *__thiscall phys_simple_allocator<gjk_brush_t>::allocate(phys_simple_allocator<gjk_brush_t> *this);
//void __thiscall phys_simple_allocator<gjk_brush_t>::free(phys_simple_allocator<gjk_brush_t> *this, gjk_brush_t *slot);
//gjk_partition_t *__thiscall phys_simple_allocator<gjk_partition_t>::allocate(
//        phys_simple_allocator<gjk_partition_t> *this);
//void __thiscall phys_simple_allocator<gjk_partition_t>::free(
//        phys_simple_allocator<gjk_partition_t> *this,
//        gjk_partition_t *slot);
//gjk_double_sphere_t *__thiscall phys_simple_allocator<gjk_double_sphere_t>::allocate(
//        phys_simple_allocator<gjk_double_sphere_t> *this);
//void __thiscall phys_simple_allocator<gjk_double_sphere_t>::free(
//        phys_simple_allocator<gjk_double_sphere_t> *this,
//        gjk_double_sphere_t *slot);
//gjk_cylinder_t *__thiscall phys_simple_allocator<gjk_cylinder_t>::allocate(phys_simple_allocator<gjk_cylinder_t> *this);
//void __thiscall phys_simple_allocator<gjk_cylinder_t>::free(
//        phys_simple_allocator<gjk_cylinder_t> *this,
//        gjk_cylinder_t *slot);
//gjk_polygon_cylinder_t *__thiscall phys_simple_allocator<gjk_polygon_cylinder_t>::allocate(
//        phys_simple_allocator<gjk_polygon_cylinder_t> *this);
//void __thiscall phys_simple_allocator<gjk_polygon_cylinder_t>::free(
//        phys_simple_allocator<gjk_polygon_cylinder_t> *this,
//        gjk_polygon_cylinder_t *slot);

struct phys_transient_allocator // sizeof=0x18
{                                       // XREF: pulse_sum_constraint_solver/r
    struct block_header // sizeof=0xC
    {
        unsigned int m_block_size;
        unsigned int m_block_alignment;
        phys_transient_allocator::block_header *m_next_block;
    };
    struct allocator_state // sizeof=0x10
    {                                       // XREF: gjk_query_output/r
        phys_transient_allocator::block_header *m_first_block;
        char *m_cur;                        // XREF: pulse_sum_constraint_solver::execute_constraint_solver(rigid_body * const)+2B9/w
        char *m_end;                        // XREF: pulse_sum_constraint_solver::execute_constraint_solver(rigid_body * const)+2CE/w
        unsigned int m_total_memory_allocated;
    };

    phys_transient_allocator::block_header *m_first_block;
    char *m_cur;                        // XREF: physics_system::time_step(float,bool)+14F/w
    char *m_end;                        // XREF: physics_system::time_step(float,bool)+152/w
    unsigned int m_total_memory_allocated;
    minspec_read_write_mutex m_mutex;   // XREF: physics_system::time_step(float,bool)+158/w
    void *m_slot_pool;                  // XREF: physics_system::time_step(float,bool)+15F/w

    void *__thiscall allocate(
        int size,
        int alignment,
        int no_error,
        const char *error_msg);

    void resize();
    void reset();

    void reset_to_state(const phys_transient_allocator::allocator_state *as);
};

struct phys_memory_heap // sizeof=0x10
{                                                                             // XREF: phys_contact_manifold_process/r
        char *m_buffer_start;
        char *m_buffer_end;
        char *m_buffer_cur;
        char *m_user_start;

        //char *phys_memory_heap::fast_allocate(phys_memory_heap *this, int size, const char *error_msg);
        char *fast_allocate(int size, const char *error_msg);
        char *fast_align_start(int alignment, const char *error_msg);
        void set_buffer(char *start, int size, unsigned int alignment);
};

template <typename T, int SIZE>
struct __declspec(align(16)) phys_static_array
{
    char m_buffer[sizeof(T) * SIZE];
    T * m_slot_array;
    int m_alloc_count;

    struct iterator//phys_static_array<phys_convex_hull::ch_edge,128>::iterator // sizeof=0x4
    {                                       // XREF: ?compute_convex_hull@phys_convex_hull@@QAEXHM@Z/r
        //phys_convex_hull::ch_edge *m_ptr;   // XREF: phys_convex_hull::add_intermediate_edge(phys_vec3 *,phys_vec3 *)+4D/w
        T *m_ptr;
    };

private:
    bool is_member(T *data)
    {
        return (data >= this->m_slot_array && data < &this->m_slot_array[this->m_alloc_count]);
    }
public:

    phys_static_array()
    {
        m_slot_array = (T*)&m_buffer;
        m_alloc_count = 0;
    }

    T *operator[](int i)
    {
        iassert(i >= 0 && i < m_alloc_count);
        return &this->m_slot_array[i];
    }

    T *add(int no_error, const char *error_msg)
    {
        if (m_alloc_count < 512)
        {
            return &this->m_slot_array[m_alloc_count++];
        }
        else
        {
            if (!no_error)
            {
                iassert(0);
                //tlFatal(error_msg);
            }
            return NULL;
        }
    }

    T *back()
    {
        iassert(m_alloc_count > 0);

        return &this->m_slot_array[m_alloc_count - 1];
    }

    T *get_list_head()
    {
        iassert(m_alloc_count > 0);

        return this->m_slot_array;
    }

    void remove_slow(T *data)
    {
        iassert(is_member(data));

        *data = &this->m_slot_array[--this->m_alloc_count];
    }
};


#define PTR_LIST_SIZE 256

// whoever wrote this class is a crackhead
template <typename T>
struct phys_free_list
{
public:
    struct T_internal_base
    {
        T_internal_base *m_prev_T_internal;
        T_internal_base *m_next_T_internal;
    };
    //static_assert(sizeof(T_internal_base) == 8);

    struct iterator//phys_free_list<broad_phase_collision_pair>::iterator // sizeof=0x4
    {
        //phys_free_list<broad_phase_collision_pair>::T_internal_base *m_ptr;
        T_internal_base *m_ptr;
    };

    struct __declspec(align(16)) T_internal : T_internal_base
    {
        T m_data;
        int m_ptr_list_index;
    };

    T_internal_base m_dummy_head;

    int m_list_count;
    int m_list_count_high_water;

    T *m_ptr_list[PTR_LIST_SIZE];
    int m_ptr_list_count;

public:

    void debug_add(T_internal *T_i)
    {
        int m_list_count; // [esp+0h] [ebp-10h]

        if (this->m_list_count_high_water <= this->m_list_count)
            m_list_count = this->m_list_count;
        else
            m_list_count = this->m_list_count_high_water;
        this->m_list_count_high_water = m_list_count;
        if (this->m_ptr_list_count >= PTR_LIST_SIZE)
        {
            T_i->m_ptr_list_index = -1;
        }
        else
        {
            T_i->m_ptr_list_index = this->m_ptr_list_count;
            this->m_ptr_list[this->m_ptr_list_count++] = &T_i->m_data;
        }
    }

    T *add(int no_error, const char *error_msg)
    {
        T_internal *ptr;

        ptr = (T_internal *)PMM_ALLOC(sizeof(T), sizeof(T) % 16 == 0 ? 16 : 4);

        if (ptr)
        {
            new ((void*) & ptr->m_data) T();
            ptr->m_prev_T_internal = &this->m_dummy_head;
            ptr->m_next_T_internal = this->m_dummy_head.m_next_T_internal;
            this->m_dummy_head.m_next_T_internal->m_prev_T_internal = ptr;
            this->m_dummy_head.m_next_T_internal = ptr;
            
            m_list_count++;

            debug_add(ptr);

            return &ptr->m_data;
        }
        else
        {
            iassert(no_error);
            return NULL;
        }
    }

    void debug_remove(T_internal *T_i)
    {
        if (T_i->m_ptr_list_index != -1)
        {
            iassert(T_i->m_ptr_list_index >= 0 && T_i->m_ptr_list_index < PTR_LIST_SIZE);
            iassert(m_ptr_list[T_i->m_ptr_list_index] == &T_i->m_data);

            //this->m_ptr_list[--this->m_ptr_list_count][1].m_wheel_state[0].m_state = T_i->m_ptr_list_index;
            this->m_ptr_list[this->m_ptr_list_count] = (T*)T_i->m_ptr_list_index;
            --this->m_ptr_list_count;
            this->m_ptr_list[T_i->m_ptr_list_index] = this->m_ptr_list[this->m_ptr_list_count];
        }
    }

    void remove(T_internal *data)
    {
        T_internal_base *next; // [esp+14h] [ebp-8h]
        T_internal_base *prev; // [esp+18h] [ebp-4h]

        iassert(data);

        --this->m_list_count;
        debug_remove(data);
        next = data->m_next_T_internal;
        prev = data->m_prev_T_internal;
        prev->m_next_T_internal = next;
        next->m_prev_T_internal = prev;
        PMM_FREE((unsigned __int8 *)data, sizeof(T), sizeof(T) % 16 == 0 ? 16 : 4);
    }

    void remove(T *data_)
    {
        if (data_)
        {
            PMM_VALIDATE((char *)data_ - (sizeof(T) % 16 == 0 ? 16 : 8), sizeof(T), (sizeof(T) % 16 == 0 ? 16 : 4));
            remove((T_internal*)((char *)data_ - (sizeof(T) % 16 == 0 ? 16 : 8)));
        }
    }

    void remove_all()
    {
        T_internal *data; // esi
        T_internal_base *m_prev_T_internal; // ecx
        T_internal_base *v4; // eax

        while ((phys_free_list<T> *)this->m_dummy_head.m_next_T_internal != this)
        {
            data = (phys_free_list<T>::T_internal *)this->m_dummy_head.m_next_T_internal;
            iassert(data);
            //if (!m_next_T_internal)
            //{
            //    if (_tlAssert("c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h", 477, "data", ""))
            //        __debugbreak();
            //}
            --this->m_list_count;
            debug_remove(data);
            m_prev_T_internal = data->m_prev_T_internal;
            v4 = data->m_next_T_internal;
            m_prev_T_internal->m_next_T_internal = v4;
            v4->m_prev_T_internal = m_prev_T_internal;
            delete (T*)(&data->m_data);
            PMM_FREE((unsigned __int8 *)data, 0x38u, 4u);
        }
    }

    void ptr_array_read(T **ptr_array, int ptr_array_size)
    {
        T_internal_base *i; // eax
        iassert(ptr_array_size == m_list_count);

        for (i = this->m_dummy_head.m_next_T_internal;
            i != (T_internal_base *)this;
            ++ptr_array)
        {
            *ptr_array = (T*)&i[1];
            i = i->m_next_T_internal;
        }
    }

    void ptr_array_write(T **ptr_array, int ptr_array_size)
    {
        T **v4; // edx
        T_internal_base *p_m_avl_key; // eax
        T**v6; // esi
        T_internal_base *v7; // ecx

        iassert(ptr_array_size == m_list_count);

        if (ptr_array_size > 0)
        {
            v4 = ptr_array;
            p_m_avl_key = (T_internal_base *) & (*ptr_array)[-1].m_avl_key;
            v6 = &ptr_array[ptr_array_size - 1];
            p_m_avl_key->m_prev_T_internal = &this->m_dummy_head;
            this->m_dummy_head.m_next_T_internal = p_m_avl_key;
            if (ptr_array < v6)
            {
                do
                {
                    v7 = (T_internal_base *)v4[1];
                    ++v4;
                    p_m_avl_key->m_next_T_internal = --v7;
                    v7->m_prev_T_internal = p_m_avl_key;
                    p_m_avl_key = v7;
                } while (v4 < v6);
            }
            p_m_avl_key->m_next_T_internal = &this->m_dummy_head;
            this->m_dummy_head.m_prev_T_internal = p_m_avl_key;
        }
    }
};

inline phys_vec3 *__cdecl phys_AbsValue(phys_vec3 *result, const phys_vec3 *a)
{
    float v3; // [esp+8h] [ebp-10h]
    float v4; // [esp+10h] [ebp-8h]

    v4 = fabsf(a->z);
    v3 = fabsf(a->y);
    result->x = fabsf(a->x);
    result->y = v3;
    result->z = v4;
    return result;
}

template <typename T>
struct phys_inplace_avl_tree_node//<auto_rigid_body> // sizeof=0xC
{                                       // XREF: auto_rigid_body/r
    //auto_rigid_body *m_left;
    //auto_rigid_body *m_right;
    T *m_left;
    T *m_right;
    int m_balance;
};


// aislop
template<typename T1, typename T2, typename Accessor>
struct phys_inplace_avl_tree
{
    struct stack_item
    {
        T2 **m_node;   // pointer to pointer to node
        int  m_child;  // -1 = left, +1 = right
    };

    T2 *m_tree_root = nullptr;

    /* ------------------------------------------------------------ */
    /* rotations                                                    */
    /* ------------------------------------------------------------ */

    inline void rotate_left(T2 **root)
    {
        T2 *save_right = (*root)->m_avl_node_info.m_right;

        (*root)->m_avl_node_info.m_right =
            save_right->m_avl_node_info.m_left;
        save_right->m_avl_node_info.m_left = *root;

        int delta = (save_right->m_avl_node_info.m_balance <= 0)
            ? 0
            : save_right->m_avl_node_info.m_balance;

        (*root)->m_avl_node_info.m_balance -= delta + 1;

        int adj = ((*root)->m_avl_node_info.m_balance >= 0)
            ? 0
            : -(*root)->m_avl_node_info.m_balance;

        save_right->m_avl_node_info.m_balance -= adj + 1;

        *root = save_right;
    }

    inline void rotate_right(T2 **root)
    {
        T2 *save_left = (*root)->m_avl_node_info.m_left;

        (*root)->m_avl_node_info.m_left =
            save_left->m_avl_node_info.m_right;
        save_left->m_avl_node_info.m_right = *root;

        int delta = (save_left->m_avl_node_info.m_balance >= 0)
            ? 0
            : -save_left->m_avl_node_info.m_balance;

        (*root)->m_avl_node_info.m_balance += delta + 1;

        int adj = ((*root)->m_avl_node_info.m_balance <= 0)
            ? 0
            : (*root)->m_avl_node_info.m_balance;

        save_left->m_avl_node_info.m_balance += adj + 1;

        *root = save_left;
    }

    /* ------------------------------------------------------------ */
    /* find                                                         */
    /* ------------------------------------------------------------ */

    inline T2 *find(const T1 *key)
    {
        T2 *node = m_tree_root;

        while (node)
        {
            if (Accessor::equals(node, key))
                return node;

            node = Accessor::less(key, node)
                ? node->m_avl_node_info.m_left
                : node->m_avl_node_info.m_right;
        }
        return nullptr;
    }

    /* ------------------------------------------------------------ */
    /* add                                                          */
    /* ------------------------------------------------------------ */

    inline void add(const T1 *key, T2 *data)
    {
        stack_item stack[32];
        stack_item *cur = stack;

        cur->m_node = &m_tree_root;

        while (*cur->m_node)
        {
            T2 *root = *cur->m_node;
            stack_item *next = cur + 1;

            if (!Accessor::less(key, root))
            {
                cur->m_child = +1;
                next->m_node = &root->m_avl_node_info.m_right;
            }
            else
            {
                cur->m_child = -1;
                next->m_node = &root->m_avl_node_info.m_left;
            }
            cur = next;
        }

        *cur->m_node = data;
        data->m_avl_node_info.m_left = nullptr;
        data->m_avl_node_info.m_right = nullptr;
        data->m_avl_node_info.m_balance = 0;

        do
        {
            if (cur <= stack)
                break;

            --cur;
            T2 **node = cur->m_node;
            (*node)->m_avl_node_info.m_balance += cur->m_child;

            if ((*node)->m_avl_node_info.m_balance == -2)
            {
                if ((*node)->m_avl_node_info.m_left
                    ->m_avl_node_info.m_balance == 1)
                {
                    rotate_left(&(*node)->m_avl_node_info.m_left);
                }
                rotate_right(node);
            }
            else if ((*node)->m_avl_node_info.m_balance == 2)
            {
                if ((*node)->m_avl_node_info.m_right
                    ->m_avl_node_info.m_balance == -1)
                {
                    rotate_right(&(*node)->m_avl_node_info.m_right);
                }
                rotate_left(node);
            }
        } while ((*cur->m_node)->m_avl_node_info.m_balance);
    }

    /* ------------------------------------------------------------ */
    /* remove                                                       */
    /* ------------------------------------------------------------ */

    inline void remove(const T1 *key)
    {
        stack_item stack[32];
        stack_item *cur = stack;

        cur->m_node = &m_tree_root;

        while (true)
        {
            T2 *root = *cur->m_node;
            stack_item *next = cur + 1;

            if (Accessor::less(key, root))
            {
                cur->m_child = -1;
                next->m_node = &root->m_avl_node_info.m_left;
            }
            else if (Accessor::less(root, key))
            {
                cur->m_child = +1;
                next->m_node = &root->m_avl_node_info.m_right;
            }
            else
            {
                break;
            }
            cur = next;
        }

        T2 **victim = cur->m_node;

        if ((*victim)->m_avl_node_info.m_right)
        {
            cur->m_child = +1;
            ++cur;
            cur->m_node = &(*victim)->m_avl_node_info.m_right;

            while ((*cur->m_node)->m_avl_node_info.m_left)
            {
                cur->m_child = -1;
                (cur + 1)->m_node =
                    &(*cur->m_node)->m_avl_node_info.m_left;
                ++cur;
            }

            T2 *replace = *cur->m_node;
            *cur->m_node = replace->m_avl_node_info.m_right;

            replace->m_avl_node_info =
                (*victim)->m_avl_node_info;

            *victim = replace;
        }
        else
        {
            *victim = (*victim)->m_avl_node_info.m_left;
        }

        do
        {
            if (cur <= stack)
                break;

            --cur;
            T2 **node = cur->m_node;
            (*node)->m_avl_node_info.m_balance -= cur->m_child;

            if ((*node)->m_avl_node_info.m_balance == -2)
            {
                if ((*node)->m_avl_node_info.m_left
                    ->m_avl_node_info.m_balance == 1)
                {
                    rotate_left(&(*node)->m_avl_node_info.m_left);
                }
                rotate_right(node);
            }
            else if ((*node)->m_avl_node_info.m_balance == 2)
            {
                if ((*node)->m_avl_node_info.m_right
                    ->m_avl_node_info.m_balance == -1)
                {
                    rotate_right(&(*node)->m_avl_node_info.m_right);
                }
                rotate_left(node);
            }
        } while ((*cur->m_node)->m_avl_node_info.m_balance == 0);
    }
};


struct __declspec(align(16)) cached_query_info_t // sizeof=0x30
{                                       // XREF: gjk_query_output/r
    phys_vec3 m_query_aabb_min;
    phys_vec3 m_query_aabb_max;
    int m_query_contents;
    unsigned int m_query_flags;
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte

    inline bool is_empty()
    {
        if (this->m_query_contents)
            return false;

        if (this->m_query_flags)
        {
            if (!Assert_MyHandler(
                "c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../physics/phys_gjk_collision_detection.h",
                212,
                0,
                "%s",
                "m_query_flags == 0"))
                __debugbreak();
        }

        return true;
    }

    bool aabb_is_valid();
    void init_query(
        const phys_vec3 *query_aabb_min,
        const phys_vec3 *query_aabb_max,
        const phys_vec3 *extra,
        int query_contents,
        unsigned int query_flags);

    void add_query(
        const phys_vec3 *query_aabb_min,
        const phys_vec3 *query_aabb_max,
        const phys_vec3 *extra,
        int query_contents,
        unsigned int query_flags);
    bool is_subset(
        const phys_vec3 *query_aabb_min,
        const phys_vec3 *query_aabb_max,
        int query_contents,
        unsigned int query_flags);
    bool is_subset_aabb(
        const phys_vec3 *query_aabb_min,
        const phys_vec3 *query_aabb_max);
};

struct __declspec(align(2)) geom_plane // sizeof=0x30
{
    phys_vec3 m_normal;
    phys_vec3 m_arm;
    float m_d;
    float m_lambda;
    float m_right_side;
    bool m_active;
    bool m_walkable;
    bool m_no_push_out;
    // padding byte
};

struct Phys_UnitQuaternion // sizeof=0x10
{                                       // XREF: ?calc_velocities@nuge@@SAXABVphys_mat44@@0MPAVphys_vec3@@1@Z/r
    float x;
    float y;
    float z;
    float w;
};

inline const phys_vec3 *__cdecl phys_min(phys_vec3 *result, const phys_vec3 *v1, const phys_vec3 *v2)
{
    float x; // [esp+0h] [ebp-24h]
    float y; // [esp+4h] [ebp-20h]
    float z; // [esp+8h] [ebp-1Ch]

    if (v2->z < v1->z)
        z = v2->z;
    else
        z = v1->z;
    if (v2->y < v1->y)
        y = v2->y;
    else
        y = v1->y;
    if (v2->x < v1->x)
        x = v2->x;
    else
        x = v1->x;
    result->x = x;
    result->y = y;
    result->z = z;
    return result;
}

inline const phys_vec3 *__cdecl phys_max(phys_vec3 *result, const phys_vec3 *v1, const phys_vec3 *v2)
{
    float x; // [esp+0h] [ebp-24h]
    float y; // [esp+4h] [ebp-20h]
    float z; // [esp+8h] [ebp-1Ch]

    if (v1->z < v2->z)
        z = v2->z;
    else
        z = v1->z;
    if (v1->y < v2->y)
        y = v2->y;
    else
        y = v1->y;
    if (v1->x < v2->x)
        x = v2->x;
    else
        x = v1->x;
    result->x = x;
    result->y = y;
    result->z = z;
    return result;
}

inline const phys_vec3 *__cdecl phys_multiply(phys_vec3 *result, const phys_mat44 *mat, const phys_vec3 *v)
{
    float v4; // [esp-ACh] [ebp-B8h]
    float v5; // [esp-A8h] [ebp-B4h]
    float y; // [esp-34h] [ebp-40h]
    float z; // [esp-4h] [ebp-10h]

    z = v->z;
    y = v->y;
    v5 = (float)((float)(v->x * mat->x.y) + (float)(y * mat->y.y)) + (float)(z * mat->z.y);
    v4 = (float)((float)(v->x * mat->x.z) + (float)(y * mat->y.z)) + (float)(z * mat->z.z);
    result->x = (float)((float)(v->x * mat->x.x) + (float)(y * mat->y.x)) + (float)(z * mat->z.x);
    result->y = v5;
    result->z = v4;
    return result;
}

inline const phys_vec3 *phys_full_multiply(
    phys_vec3 *result,
    const phys_mat44 *mat,
    const phys_vec3 *v)
{
    //phys_vec3 *result;
    float v5; // [esp-30h] [ebp-3Ch]
    float v6; // [esp-2Ch] [ebp-38h]
    const phys_vec3 *v7; // [esp-24h] [ebp-30h]
    phys_vec3 v8; // [esp-20h] [ebp-2Ch] BYREF
    const phys_vec3 *p_w; // [esp-4h] [ebp-10h]
    //int v10; // [esp+0h] [ebp-Ch]
    //void *v11; // [esp+4h] [ebp-8h]
    //void *retaddr; // [esp+Ch] [ebp+0h]

    //v10 = a1;
    //v11 = retaddr;
    p_w = &mat->w;
    v7 = phys_multiply(&v8, mat, v);
    v6 = v7->y + p_w->y;
    v5 = v7->z + p_w->z;
    result->x = v7->x + p_w->x;
    result->y = v6;
    result->z = v5;
    return result;
}

inline double __cdecl phys_dot(const phys_vec3 *a, const phys_vec3 *b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

inline void __cdecl phys_transpose(phys_mat44 *dest, const phys_mat44 *source);
inline void phys_calc_world_aabb(
    const phys_vec3 *local_center,
    const phys_vec3 *local_half_aabb_dims,
    const phys_mat44 *local_to_world_xform,
    phys_vec3 *aabb_min,
    phys_vec3 *aabb_max)
{
    const phys_vec3 *v6; // eax
    float v7; // [esp-48h] [ebp-108h]
    float v8; // [esp-44h] [ebp-104h]
    float v9; // [esp-28h] [ebp-E8h]
    float v10; // [esp-24h] [ebp-E4h]
    phys_vec3 v11; // [esp-Ch] [ebp-CCh] BYREF
    float center_4; // [esp+4h] [ebp-BCh]
    float center_8; // [esp+8h] [ebp-B8h]
    float center_12; // [esp+Ch] [ebp-B4h]
    float v15; // [esp+20h] [ebp-A0h]
    phys_vec3 v16; // [esp+24h] [ebp-9Ch] BYREF
    float v17; // [esp+3Ch] [ebp-84h]
    phys_vec3 *v18; // [esp+40h] [ebp-80h]
    phys_vec3 v19; // [esp+44h] [ebp-7Ch] BYREF
    float v20; // [esp+5Ch] [ebp-64h]
    phys_vec3 *v21; // [esp+60h] [ebp-60h]
    phys_vec3 v22; // [esp+64h] [ebp-5Ch] BYREF
    phys_mat44 v23; // [esp+74h] [ebp-4Ch] BYREF
    //_DWORD world_to_local_xform_52[3]; // [esp+B4h] [ebp-Ch] BYREF
    //_UNKNOWN *retaddr; // [esp+C0h] [ebp+0h]

    //*(float *)world_to_local_xform_52 = a1;
    //world_to_local_xform_52[1] = retaddr;
    phys_transpose(&v23, local_to_world_xform);
    v21 = phys_AbsValue(&v22, &v23.z);
    v20 = (float)((float)(v21->x * local_half_aabb_dims->x) + (float)(v21->y * local_half_aabb_dims->y))
        + (float)(v21->z * local_half_aabb_dims->z);
    v18 = phys_AbsValue(&v19, &v23.y);
    v17 = (float)((float)(v18->x * local_half_aabb_dims->x) + (float)(v18->y * local_half_aabb_dims->y))
        + (float)(v18->z * local_half_aabb_dims->z);
    v6 = phys_AbsValue(&v16, &v23.x);
    v15 = phys_dot(v6, local_half_aabb_dims);
    center_4 = v15;
    center_8 = v17;
    center_12 = v20;
    phys_full_multiply(&v11, local_to_world_xform, local_center);
    v9 = v11.y - center_8;
    v10 = v11.z - center_12;
    aabb_min->x = v11.x - center_4;
    aabb_min->y = v9;
    aabb_min->z = v10;
    v7 = v11.y + center_8;
    v8 = v11.z + center_12;
    aabb_max->x = v11.x + center_4;
    aabb_max->y = v7;
    aabb_max->z = v8;
}

inline void __cdecl phys_transpose(phys_mat44 *dest, const phys_mat44 *source)
{
    float *v2; // [esp+8h] [ebp-24h]
    float *v3; // [esp+Ch] [ebp-20h]
    float v4; // [esp+10h] [ebp-1Ch]
    float *v5; // [esp+14h] [ebp-18h]
    float *v6; // [esp+18h] [ebp-14h]
    float v7; // [esp+1Ch] [ebp-10h]
    float *v8; // [esp+24h] [ebp-8h]
    float v9; // [esp+28h] [ebp-4h]

    if (dest == source)
    {
        //v8 = phys_vec3::operator[]<int>(&dest->x, 1u);
        v8 = &dest->x[1];
        v9 = *v8;
        *v8 = dest->y.x;
        dest->y.x = v9;
        //v5 = phys_vec3::operator[]<int>(&dest->z, 0);
        v5 = &dest->z[0];
        //v6 = phys_vec3::operator[]<int>(&dest->x, 2u);
        v6 = &dest->x[2];
        v7 = *v6;
        *v6 = *v5;
        *v5 = v7;
        //v2 = phys_vec3::operator[]<int>(&dest->z, 1u);
        v2 = &dest->z[1];
        //v3 = phys_vec3::operator[]<int>(&dest->y, 2u);
        v3 = &dest->y[2];
        v4 = *v3;
        *v3 = *v2;
        *v2 = v4;
    }
    else
    {
        dest->x.x = source->x.x;
        dest->x.y = source->y.x;
        dest->x.z = source->z.x;
        dest->y.x = source->x.y;
        dest->y.y = source->y.y;
        dest->y.z = source->z.y;
        dest->z.x = source->x.z;
        dest->z.y = source->y.z;
        dest->z.z = source->z.z;
    }
}

inline const phys_vec3 *__cdecl phys_inv_multiply(phys_vec3 *result, const phys_mat44 *mat, const phys_vec3 *v)
{
    float v4; // [esp+4h] [ebp-10h]
    float v5; // [esp+Ch] [ebp-8h]

    v5 = (float)((float)(v->x * mat->z.x) + (float)(v->y * mat->z.y)) + (float)(v->z * mat->z.z);
    v4 = (float)((float)(v->x * mat->y.x) + (float)(v->y * mat->y.y)) + (float)(v->z * mat->y.z);
    result->x = (float)((float)(v->x * mat->x.x) + (float)(v->y * mat->x.y)) + (float)(v->z * mat->x.z);
    result->y = v4;
    result->z = v5;
    return result;
}

inline void __cdecl phys_calc_local_aabb(
    const phys_vec3 *aabb_min,
    const phys_vec3 *aabb_max,
    const phys_mat44 *local_to_world_xform,
    phys_vec3 *local_aabb_min,
    phys_vec3 *local_aabb_max)
{
    const phys_vec3 *v5; // eax
    const phys_vec3 *v6; // eax
    const phys_vec3 *v7; // eax
    float v8; // [esp+1F4h] [ebp-13Ch]
    float v9; // [esp+1F8h] [ebp-138h]
    float v10; // [esp+214h] [ebp-11Ch]
    float v11; // [esp+218h] [ebp-118h]
    phys_vec3 v12; // [esp+230h] [ebp-100h] BYREF
    phys_vec3 v; // [esp+240h] [ebp-F0h] BYREF
    float v14; // [esp+254h] [ebp-DCh]
    float v15; // [esp+258h] [ebp-D8h]
    float v16; // [esp+25Ch] [ebp-D4h]
    float v17; // [esp+260h] [ebp-D0h]
    float v18; // [esp+264h] [ebp-CCh]
    float v19; // [esp+268h] [ebp-C8h]
    float v20; // [esp+274h] [ebp-BCh]
    float v21; // [esp+278h] [ebp-B8h]
    float v22; // [esp+27Ch] [ebp-B4h]
    float v23; // [esp+280h] [ebp-B0h]
    float v24; // [esp+284h] [ebp-ACh]
    float v25; // [esp+288h] [ebp-A8h]
    float v26; // [esp+29Ch] [ebp-94h]
    phys_vec3 v27; // [esp+2A0h] [ebp-90h] BYREF
    float v28; // [esp+2BCh] [ebp-74h]
    phys_vec3 v29; // [esp+2C0h] [ebp-70h] BYREF
    float v30; // [esp+2DCh] [ebp-54h]
    phys_vec3 result; // [esp+2E0h] [ebp-50h] BYREF
    phys_vec3 b; // [esp+2F0h] [ebp-40h] BYREF
    float v33; // [esp+304h] [ebp-2Ch]
    float v34; // [esp+308h] [ebp-28h]
    float v35; // [esp+30Ch] [ebp-24h]
    float v36; // [esp+310h] [ebp-20h]
    float v37; // [esp+314h] [ebp-1Ch]
    float v38; // [esp+318h] [ebp-18h]
    float v39; // [esp+324h] [ebp-Ch]
    float v40; // [esp+328h] [ebp-8h]
    float v41; // [esp+32Ch] [ebp-4h]
    int savedregs; // [esp+330h] [ebp+0h] BYREF

    v41 = aabb_max->x - aabb_min->x;
    v40 = aabb_max->y - aabb_min->y;
    v39 = aabb_max->z - aabb_min->z;
    v36 = v41;
    v37 = v40;
    v38 = v39;
    v35 = 0.5 * v41;
    v34 = 0.5 * v40;
    v33 = 0.5 * v39;
    b.x = 0.5 * v41;
    b.y = 0.5 * v40;
    b.z = 0.5 * v39;
    v5 = phys_AbsValue(&result, &local_to_world_xform->z);
    v30 = phys_dot(v5, &b);
    v6 = phys_AbsValue(&v29, &local_to_world_xform->y);
    v28 = phys_dot(v6, &b);
    v7 = phys_AbsValue(&v27, &local_to_world_xform->x);
    v26 = phys_dot(v7, &b);
    v23 = v26;
    v24 = v28;
    v25 = v30;
    v22 = aabb_min->x + aabb_max->x;
    v21 = aabb_min->y + aabb_max->y;
    v20 = aabb_min->z + aabb_max->z;
    v17 = v22;
    v18 = v21;
    v19 = v20;
    v16 = 0.5 * v22;
    v15 = 0.5 * v21;
    v14 = 0.5 * v20;
    v.x = 0.5 * v22;
    v.y = 0.5 * v21;
    v.z = 0.5 * v20;
    phys_full_inv_multiply(&v12, local_to_world_xform, &v);
    v10 = v12.y - v24;
    v11 = v12.z - v25;
    local_aabb_min->x = v12.x - v23;
    local_aabb_min->y = v10;
    local_aabb_min->z = v11;
    v8 = v12.y + v24;
    v9 = v12.z + v25;
    local_aabb_max->x = v12.x + v23;
    local_aabb_max->y = v8;
    local_aabb_max->z = v9;
}

void phys_multiply_mat(phys_mat44 *dest, const phys_mat44 *left, const phys_mat44 *right);
phys_vec3 *phys_cross(phys_vec3 *result, const phys_vec3 *a, const phys_vec3 *b);
void phys_full_multiply_mat(phys_mat44 *dest, const phys_mat44 *left, const phys_mat44 *right);
void Phys_Mat4ToNitrousMat(float (*inMat)[3], phys_mat44 *outMat);
void Phys_NitrousMat44ToVec33(const phys_mat44 *inMat, float (*outAxis)[3]);

void make_rotate(
    phys_mat44 *mat,
    const phys_vec3 *v,
    float theta_factor,
    float max_rotation_radians);
//void    make_rotate(
//    phys_mat44 *mat,
//    const phys_vec3 *v,
//    float theta_factor,
//    float max_rotation_radians);

void make_rotate(phys_mat44 *m, const phys_vec3 *u, float ca, float sa);
//void __cdecl make_rotate(phys_mat44 *m, const phys_vec3 *u, float ca, float sa);

void make_rotate(phys_mat44 *mat, const phys_vec3 *v1, const phys_vec3 *v2);
//void make_rotate(phys_mat44 *mat, const phys_vec3 *v1, const phys_vec3 *v2);


// dumb
inline void __cdecl Phys_Vec3ToNitrousVec(const float * const inVector, phys_vec3 *outVector)
{
    outVector->x = inVector[0];
    outVector->y = inVector[1];
    outVector->z = inVector[2];
}

inline void __cdecl Phys_AxisToNitrousMat(float (*axis)[3], phys_mat44 *outMat)
{
    Phys_Vec3ToNitrousVec((float *)axis, &outMat->x);
    Phys_Vec3ToNitrousVec(&(*axis)[3], &outMat->y);
    Phys_Vec3ToNitrousVec(&(*axis)[6], &outMat->z);
}


// oh fuck yes, in the compiler this is slurped into every file and duplicated 68 times
static const phys_vec3 PHYS_X_VEC = { 1.0f, 0.0f, 0.0f, 0.0f };
static const phys_vec3 PHYS_Y_VEC = { 0.0f, 1.0f, 0.0f, 0.0f };
static const phys_vec3 PHYS_Z_VEC = { 0.0f, 0.0f, 1.0f, 0.0f };

static const phys_vec3 PHYS_ZERO_VEC = { 0.0f, 0.0f, 0.0f, 0.0f };

static const phys_mat44 PHYS_IDENTITY_MATRIX(&PHYS_X_VEC, &PHYS_Y_VEC, &PHYS_Z_VEC, &PHYS_ZERO_VEC);

static const float PHYS_PI = 3.1415927f;
static const float PHYS_PI_TIMES_2 = 6.2831855f;
static const float PHYS_PI_OVER_2 = 1.5707964f;

inline bool _tlAssert(const char *filename, int line, const char *msg, const char *msg2)
{
    return true;
}

#define PHYS_ALIGNOF(type) alignof(type)
