type JsonScalar__DARKLUA_TYPE_a=string|number|boolean|nil type
JsonValue__DARKLUA_TYPE_b=JsonScalar__DARKLUA_TYPE_a|JsonObject__DARKLUA_TYPE_c|
JsonArray__DARKLUA_TYPE_d type JsonObject__DARKLUA_TYPE_c={[string]:
JsonValue__DARKLUA_TYPE_b}type JsonArray__DARKLUA_TYPE_d={
JsonValue__DARKLUA_TYPE_b}type PayloadObject__DARKLUA_TYPE_e={[string]:string}
type MapperParams__DARKLUA_TYPE_f={payload:PayloadObject__DARKLUA_TYPE_e,
field_subset:JsonObject__DARKLUA_TYPE_c?}type PutFunction__DARKLUA_TYPE_g=(
JsonObject__DARKLUA_TYPE_c,string,JsonValue__DARKLUA_TYPE_b?)->()type
SafeStringFunction__DARKLUA_TYPE_h=(JsonValue__DARKLUA_TYPE_b?)->
JsonValue__DARKLUA_TYPE_b type SafeNumberFunction__DARKLUA_TYPE_i=(
JsonValue__DARKLUA_TYPE_b?)->JsonValue__DARKLUA_TYPE_b type
TimestampFunction__DARKLUA_TYPE_j=()->string type
BoolFromIntFunction__DARKLUA_TYPE_k=(number?)->boolean?type
CleanTextFunction__DARKLUA_TYPE_l=(string?)->string?type
JsonValue__DARKLUA_TYPE_m=JsonValue__DARKLUA_TYPE_b type
CommonConstantsTable__DARKLUA_TYPE_n={NULL:JsonValue__DARKLUA_TYPE_m,EMPTY_ARRAY
:JsonValue__DARKLUA_TYPE_m,DEFAULT_VERSION:string,COMMON_NULL_REPRESENTATIONS:{
string},is_null_like:(JsonValue__DARKLUA_TYPE_m?)->boolean}type
JsonValue__DARKLUA_TYPE_o=JsonValue__DARKLUA_TYPE_b type
JsonObject__DARKLUA_TYPE_p=JsonObject__DARKLUA_TYPE_c type
JsonArray__DARKLUA_TYPE_q=JsonArray__DARKLUA_TYPE_d type
NullModule__DARKLUA_TYPE_r={convert_nulls:(JsonValue__DARKLUA_TYPE_o)->
JsonValue__DARKLUA_TYPE_o,is_null_marker:(JsonValue__DARKLUA_TYPE_o)->boolean,
is_empty_array_marker:(JsonValue__DARKLUA_TYPE_o)->boolean,prepare_for_json:(
JsonValue__DARKLUA_TYPE_o)->JsonValue__DARKLUA_TYPE_o}type
JsonValue__DARKLUA_TYPE_s=JsonValue__DARKLUA_TYPE_b type
JsonObject__DARKLUA_TYPE_t=JsonObject__DARKLUA_TYPE_c type
CommonHelpersTable__DARKLUA_TYPE_u={NULL:JsonValue__DARKLUA_TYPE_s,EMPTY_ARRAY:
JsonValue__DARKLUA_TYPE_s,put:(JsonObject__DARKLUA_TYPE_t,string,
JsonValue__DARKLUA_TYPE_s?)->(),safe_string:(JsonValue__DARKLUA_TYPE_s?)->
JsonValue__DARKLUA_TYPE_s,safe_string_trimmed:(JsonValue__DARKLUA_TYPE_s?)->
JsonValue__DARKLUA_TYPE_s,safe_string_preserve_newlines:(
JsonValue__DARKLUA_TYPE_s?)->JsonValue__DARKLUA_TYPE_s,safe_number:(
JsonValue__DARKLUA_TYPE_s?)->JsonValue__DARKLUA_TYPE_s,bool_from_int:(number|
boolean|nil)->boolean?,clean_text:(string?)->string?,get_timestamp:()->string,
is_null_like:(JsonValue__DARKLUA_TYPE_s?)->boolean,prepare_for_json:(
JsonValue__DARKLUA_TYPE_s)->JsonValue__DARKLUA_TYPE_s,parse_json_string:(string?
)->JsonValue__DARKLUA_TYPE_s?}type JsonValue__DARKLUA_TYPE_v=
JsonValue__DARKLUA_TYPE_b type JsonObject__DARKLUA_TYPE_w=
JsonObject__DARKLUA_TYPE_c type ErrorInfo__DARKLUA_TYPE_x={error_code:string,
error_message:string,platform:string,timestamp:string,context:
JsonObject__DARKLUA_TYPE_w?}type ValidationResult__DARKLUA_TYPE_y={is_valid:
boolean,errors:{string}}type SafeCallResult__DARKLUA_TYPE_z<T> ={success:boolean
,result:T?,error:string?}type ErrorHandlerTable__DARKLUA_TYPE_A={safe_call:<T>((
()->T),string?)->SafeCallResult__DARKLUA_TYPE_z<T>,validate_input:(
JsonValue__DARKLUA_TYPE_v?,string)->ValidationResult__DARKLUA_TYPE_y,
with_time_budget:<T>((()->T),number,string)->SafeCallResult__DARKLUA_TYPE_z<T>,
log_error:(ErrorInfo__DARKLUA_TYPE_x)->(),recover_with_default:<T>(
SafeCallResult__DARKLUA_TYPE_z<T>,T)->T}type JsonValue__DARKLUA_TYPE_B=
JsonValue__DARKLUA_TYPE_b type JsonObject__DARKLUA_TYPE_C=
JsonObject__DARKLUA_TYPE_c type Timer__DARKLUA_TYPE_D={operation:string,platform
:string,start_time:number,start_memory:number?}type MetricEntry__DARKLUA_TYPE_E=
{timestamp:string,platform:string,operation:string,duration_ms:number,
memory_used_kb:number?,success:boolean,error_message:string?}type
ResourceUsage__DARKLUA_TYPE_F={memory_kb:number,execution_time_ms:number,
iterations:number?}type MetricsTable__DARKLUA_TYPE_G={start_timer:(string,string
)->Timer__DARKLUA_TYPE_D,end_timer:(Timer__DARKLUA_TYPE_D,boolean?,string?)->
MetricEntry__DARKLUA_TYPE_E,record_counter:(string,string,number?)->(),
record_histogram:(string,string,number)->(),get_memory_usage:()->number,
log_metric:(MetricEntry__DARKLUA_TYPE_E)->(),get_performance_summary:(string)->
JsonObject__DARKLUA_TYPE_C,with_monitoring:<T>((any)->T,string,string,any)->T}
type JsonValue__DARKLUA_TYPE_H=JsonValue__DARKLUA_TYPE_b type
JsonObject__DARKLUA_TYPE_I=JsonObject__DARKLUA_TYPE_c type
JsonArray__DARKLUA_TYPE_J=JsonArray__DARKLUA_TYPE_d type
SubsetModule__DARKLUA_TYPE_K={is_array:(any)->boolean,apply_subsetting:(
JsonValue__DARKLUA_TYPE_H,JsonValue__DARKLUA_TYPE_H)->JsonValue__DARKLUA_TYPE_H?
}type JsonValue__DARKLUA_TYPE_L=JsonValue__DARKLUA_TYPE_b type
JsonObject__DARKLUA_TYPE_M=JsonObject__DARKLUA_TYPE_c type
JsonArray__DARKLUA_TYPE_N=JsonArray__DARKLUA_TYPE_d type
PayloadObject__DARKLUA_TYPE_O=PayloadObject__DARKLUA_TYPE_e type
MapperParams__DARKLUA_TYPE_P=MapperParams__DARKLUA_TYPE_f type
MapperConfig__DARKLUA_TYPE_Q={platform:string,constants:{PLATFORM:string,[string
]:any},process:(raw:PayloadObject__DARKLUA_TYPE_O,helpers:any)->
JsonObject__DARKLUA_TYPE_M,helpers:any?,timeout:number?,metrics_config:{detailed
:boolean?}?}type ProcessResult__DARKLUA_TYPE_R={success:boolean,data:
JsonObject__DARKLUA_TYPE_M?,error:string?}type
MapperTemplateModule__DARKLUA_TYPE_S={create_mapper:(config:
MapperConfig__DARKLUA_TYPE_Q)->(params:MapperParams__DARKLUA_TYPE_P)->
JsonObject__DARKLUA_TYPE_M,_process_with_config:(config:
MapperConfig__DARKLUA_TYPE_Q,raw:PayloadObject__DARKLUA_TYPE_O)->
ProcessResult__DARKLUA_TYPE_R}type DateModule__DARKLUA_TYPE_T={format_iso_date:(
number,number,number)->string,validate_iso_z:(string?)->string?,
normalize_iso_to_z:(string?)->string?,parse_us_date_inline:(string?)->string?,
unix_to_iso:(number?)->string?}type JsonValue__DARKLUA_TYPE_U=
JsonValue__DARKLUA_TYPE_b type JsonObject__DARKLUA_TYPE_V=
JsonObject__DARKLUA_TYPE_c type JsonArray__DARKLUA_TYPE_W=
JsonArray__DARKLUA_TYPE_d type HelpersTable__DARKLUA_TYPE_X={normalize_example:(
value:string?)->string?}local __BUNDLE_MODULES __BUNDLE_MODULES={cache={},load=
function(m)if not __BUNDLE_MODULES.cache[m]then __BUNDLE_MODULES.cache[m]={c=
__BUNDLE_MODULES[m]()}end return __BUNDLE_MODULES.cache[m].c end}do function
__BUNDLE_MODULES.a()return{}end function __BUNDLE_MODULES.b()local types=
__BUNDLE_MODULES.load('a')local constants:CommonConstantsTable__DARKLUA_TYPE_n={
}constants.NULL={__type='null'}constants.EMPTY_ARRAY={__type='array'}constants.
DEFAULT_VERSION='v1'constants.COMMON_NULL_REPRESENTATIONS={'','<null>','null',
'undefined'}function constants.is_null_like(value:JsonValue__DARKLUA_TYPE_m?):
boolean if value==nil then return true end if type(value)=='userdata'then return
true end if type(value)=='string'then local trimmed=value:gsub('^%s+',''):gsub(
'%s+$','')if trimmed==''then return true end for _,null_repr in ipairs(constants
.COMMON_NULL_REPRESENTATIONS)do if trimmed:lower()==null_repr:lower()then return
true end end end return false end return constants end function __BUNDLE_MODULES
.c()local types=__BUNDLE_MODULES.load('a')local constants=__BUNDLE_MODULES.load(
'b')local null:NullModule__DARKLUA_TYPE_r={}function null.is_null_marker(value:
JsonValue__DARKLUA_TYPE_o):boolean if type(value)~='table'then return false end
local obj=value::JsonObject__DARKLUA_TYPE_p return obj.__type=='null'end
function null.is_empty_array_marker(value:JsonValue__DARKLUA_TYPE_o):boolean if
type(value)~='table'then return false end local obj=value::
JsonObject__DARKLUA_TYPE_p return obj.__type=='array'end function null.
convert_nulls(value:JsonValue__DARKLUA_TYPE_o):JsonValue__DARKLUA_TYPE_o if
value==nil then return nil end if type(value)=='table'then local is_array=true
local max_index=0 local count=0 for k,_ in pairs(value)do count=count+1 if type(
k)~='number'or k~=math.floor(k)or k<1 then is_array=false break end max_index=
math.max(max_index,k)end if is_array and count>0 then is_array=(max_index==count
)end if is_array then local arr=value::JsonArray__DARKLUA_TYPE_q local new_array
:JsonArray__DARKLUA_TYPE_q={}for i,item in ipairs(arr)do new_array[i]=null.
convert_nulls(item)end return new_array else local obj=value::
JsonObject__DARKLUA_TYPE_p if null.is_null_marker(obj)then return nil::
JsonValue__DARKLUA_TYPE_o elseif null.is_empty_array_marker(obj)then return{}::
JsonArray__DARKLUA_TYPE_q else local new_obj:JsonObject__DARKLUA_TYPE_p={}for
key,val in pairs(obj)do local converted_val=null.convert_nulls(val)if
converted_val~=nil then new_obj[key]=converted_val else new_obj[key]=nil end end
return new_obj end end else return value end end function null.prepare_for_json(
value:JsonValue__DARKLUA_TYPE_o):JsonValue__DARKLUA_TYPE_o return null.
convert_nulls(value)end return null end function __BUNDLE_MODULES.d()local types
=__BUNDLE_MODULES.load('a')local constants=__BUNDLE_MODULES.load('b')local
null_handler=__BUNDLE_MODULES.load('c')local helpers:
CommonHelpersTable__DARKLUA_TYPE_u={}helpers.NULL=constants.NULL helpers.
EMPTY_ARRAY=constants.EMPTY_ARRAY function helpers.put(dest:
JsonObject__DARKLUA_TYPE_t,key:string,val:JsonValue__DARKLUA_TYPE_s?)if val~=nil
and val~=''and val~=constants.NULL then dest[key]=val end end function helpers.
safe_string(value:JsonValue__DARKLUA_TYPE_s?):JsonValue__DARKLUA_TYPE_s if
constants.is_null_like(value)then return constants.NULL end if type(value)==
'string'then return value end return tostring(value)end function helpers.
safe_string_trimmed(value:JsonValue__DARKLUA_TYPE_s?):JsonValue__DARKLUA_TYPE_s
if constants.is_null_like(value)then return constants.NULL end if type(value)~=
'string'then return constants.NULL end local trimmed=value:gsub('^%s+',''):gsub(
'%s+$','')if trimmed==''then return constants.NULL end return trimmed end
function helpers.safe_string_preserve_newlines(value:JsonValue__DARKLUA_TYPE_s?)
:JsonValue__DARKLUA_TYPE_s if constants.is_null_like(value)then return constants
.NULL end if type(value)~='string'then return constants.NULL end local s=value s
=s:gsub('^%s+',''):gsub('%s+$','')if s==''then return constants.NULL end return
s end function helpers.safe_number(value:JsonValue__DARKLUA_TYPE_s?):
JsonValue__DARKLUA_TYPE_s if constants.is_null_like(value)then return constants.
NULL end if type(value)=='number'then return value end local s:string if type(
value)=='string'then s=value:gsub(',',''):gsub('^%s+',''):gsub('%s+$','')else s=
tostring(value)end local n=tonumber(s)if n~=nil then return n end return
constants.NULL end function helpers.bool_from_int(value:number|boolean|nil):
boolean?if value==nil then return nil end if type(value)=='boolean'then return
value end if type(value)=='number'then return value~=0 end return nil end
function helpers.clean_text(text:string?):string?return text and text:gsub('%s+'
,' '):gsub('^%s+',''):gsub('%s+$','')or text end function helpers.get_timestamp(
):string return os.date('!%Y-%m-%dT%H:%M:%SZ')end helpers.is_null_like=constants
.is_null_like helpers.prepare_for_json=null_handler.prepare_for_json function
helpers.parse_json_string(json_str:string?):JsonValue__DARKLUA_TYPE_s?if not
json_str or json_str==''then return nil end local success,result=pcall(function(
)return _G.parse_json(json_str)end)if success then return result else return nil
end end return helpers end function __BUNDLE_MODULES.e()local types=
__BUNDLE_MODULES.load('a')local constants=__BUNDLE_MODULES.load('b')local
error_handler:ErrorHandlerTable__DARKLUA_TYPE_A={}error_handler.silent_mode=
false error_handler.LOG_LEVELS={DEBUG=0,INFO=1,WARN=2,ERROR=3,FATAL=4}
error_handler.log_level=error_handler.LOG_LEVELS.ERROR function error_handler.
safe_call<T>(fn:()->T,context:string?):SafeCallResult__DARKLUA_TYPE_z<T>local
success,result=pcall(fn)if success then return{success=true,result=result,error=
nil}else local error_msg=tostring(result)error_handler.log_error({error_code=
'FUNCTION_CALL_FAILED',error_message=error_msg,platform=context or'unknown',
timestamp=os.date('!%Y-%m-%dT%H:%M:%SZ'),context={operation=context}})return{
success=false,result=nil,error=error_msg}end end function error_handler.
validate_input(input:JsonValue__DARKLUA_TYPE_v?,platform:string):
ValidationResult__DARKLUA_TYPE_y local errors:{string}={}if not input then table
.insert(errors,'Input is nil')return{is_valid=false,errors=errors}end if type(
input)~='table'then table.insert(errors,'Input must be a table, got: '..type(
input))return{is_valid=false,errors=errors}end local input_table=input::
JsonObject__DARKLUA_TYPE_w local has_valid_fields=false for key,value in pairs(
input_table)do if key and value~=nil then has_valid_fields=true break end end if
not has_valid_fields then table.insert(errors,
'Input data is empty or contains no valid fields')return{is_valid=false,errors=
errors}end if platform=='github'or platform=='github_profile'then local
has_direct_fields=input_table.login or input_table.id or input_table.name or
input_table.email local has_body_structure=input_table.body and type(input_table
.body)=='table'if not has_direct_fields and not has_body_structure then table.
insert(errors,
[[Invalid GitHub data structure: missing required user fields (login, id, name, or email)]]
)end if not has_body_structure and(input_table.bad or input_table.invalid)then
table.insert(errors,
[[Data contains unexpected fields that don't match GitHub API response structure]]
)end elseif platform=='netflix'or platform=='netflix_billing'then local
has_direct_fields=input_table.billingDate or input_table.planType or input_table
.nextBillingDate or input_table.accountDetails or input_table.membershipStatus
local has_body_structure=input_table.body and type(input_table.body)=='table'if
not has_direct_fields and not has_body_structure then table.insert(errors,
[[Invalid Netflix data structure: missing required billing/account fields]])end
elseif platform=='reddit'or platform=='reddit_profile'then local
has_direct_fields=input_table.name or input_table.id or input_table.created or
input_table.link_karma or input_table.comment_karma local has_body_structure=
input_table.body and type(input_table.body)=='table'if not has_direct_fields and
not has_body_structure then table.insert(errors,
[[Invalid Reddit data structure: missing required user profile fields]])end
elseif platform=='robinhood'or platform=='robinhood_profile'or platform==
'robinhood_accounts'then local has_direct_fields=input_table.user or input_table
.username or input_table.email or input_table.results or input_table.
crypto_accounts local has_body_structure=input_table.body and type(input_table.
body)=='table'if not has_direct_fields and not has_body_structure then table.
insert(errors,
[[Invalid Robinhood data structure: missing required user or account fields]])
end elseif platform=='epic_playtime'or platform=='epic_wishlist'then local
has_direct_fields=input_table.data or input_table.games or input_table.playtime
or input_table.wishlist or input_table.elements local has_body_structure=
input_table.body and type(input_table.body)=='table'if not has_direct_fields and
not has_body_structure then table.insert(errors,
[[Invalid Epic Store data structure: missing required game or wishlist fields]])
end elseif platform=='steam'or platform=='steam_games'then local is_html=type(
input_table)=='string'or(input_table.body and type(input_table.body)=='string')
local has_game_data=input_table.response or input_table.games or input_table.
rgGames if not is_html and not has_game_data then table.insert(errors,
[[Invalid Steam data structure: expected HTML content or game data fields]])end
elseif platform=='uber'then local has_direct_fields=input_table.trips or
input_table.data or input_table.firstName or input_table.payload local
has_body_structure=false if input_table.body and type(input_table.body)=='table'
then local body=input_table.body has_body_structure=body.payload or body.trips
or body.firstName or(body.data and type(body.data)=='table')end if not
has_direct_fields and not has_body_structure then table.insert(errors,
[[Invalid Uber data structure: missing required trip or profile fields]])end
elseif platform=='test'then if next(input_table)==nil then table.insert(errors,
'Test data cannot be empty')end else local recognized_field_found=false local
common_fields={'id','name','email','username','user','data','results','items',
'profile','account','status','created','updated','timestamp','firstName',
'lastName','displayName','login','valid','test'}for _,field in ipairs(
common_fields)do if input_table[field]~=nil then recognized_field_found=true
break end end if not recognized_field_found then table.insert(errors,string.
format(
[[Data structure doesn't match expected format for platform '%s': no recognized fields found]]
,platform))end end return{is_valid=#errors==0,errors=errors}end function
error_handler.with_time_budget<T>(fn:()->T,max_seconds:number,context:string):
SafeCallResult__DARKLUA_TYPE_z<T>local start_time=os.clock()local function
bounded_fn():T local elapsed=os.clock()-start_time if elapsed>max_seconds then
error('Maximum execution time exceeded: '..max_seconds..'s in context: '..
context)end return fn()end return error_handler.safe_call(bounded_fn,context)end
function error_handler.log_error(error_info:ErrorInfo__DARKLUA_TYPE_x)local
log_entry=string.format('[ERROR] %s | %s | %s | %s',error_info.timestamp,
error_info.platform,error_info.error_code,error_info.error_message)if error_info
.context and next(error_info.context)then local context_parts={}for k,v in
pairs(error_info.context)do table.insert(context_parts,string.format('%s=%s',k,
tostring(v)))end log_entry=log_entry..' | context: '..table.concat(context_parts
,', ')end if not error_handler.silent_mode then if io and io.stderr then io.
stderr:write(log_entry..'\n')io.stderr:flush()else print(log_entry)end end end
function error_handler.recover_with_default<T>(result:
SafeCallResult__DARKLUA_TYPE_z<T>,default_value:T):T if result.success and
result.result then return result.result else return default_value end end return
error_handler end function __BUNDLE_MODULES.f()local types=__BUNDLE_MODULES.
load('a')local metrics:MetricsTable__DARKLUA_TYPE_G={}metrics.silent_mode=false
local metric_history:{MetricEntry__DARKLUA_TYPE_E}={}local counters:{[string]:
number}={}local histograms:{[string]:{number}}={}function metrics.start_timer(
operation:string,platform:string):Timer__DARKLUA_TYPE_D local memory_kb=nil
local success,count_kb=pcall(function()return collectgarbage('count')end)if
success then memory_kb=count_kb end return{operation=operation,platform=platform
,start_time=os.clock(),start_memory=memory_kb}end function metrics.end_timer(
timer:Timer__DARKLUA_TYPE_D,success:boolean?,error_message:string?):
MetricEntry__DARKLUA_TYPE_E local end_time=os.clock()local duration_ms=(end_time
-timer.start_time)*1000 local memory_used_kb=nil if timer.start_memory then
local current_memory_kb=metrics.get_memory_usage()if current_memory_kb>0 then
memory_used_kb=math.max(0,current_memory_kb-timer.start_memory)end end local
metric_entry:MetricEntry__DARKLUA_TYPE_E={timestamp=os.date(
'!%Y-%m-%dT%H:%M:%SZ'),platform=timer.platform,operation=timer.operation,
duration_ms=duration_ms,memory_used_kb=memory_used_kb,success=success~=false,
error_message=error_message}metrics.log_metric(metric_entry)return metric_entry
end function metrics.record_counter(metric_name:string,platform:string,increment
:number?)local key=platform..':'..metric_name local inc=increment or 1 counters[
key]=(counters[key]or 0)+inc end function metrics.record_histogram(metric_name:
string,platform:string,value:number)local key=platform..':'..metric_name if not
histograms[key]then histograms[key]={}end table.insert(histograms[key],value)if#
histograms[key]>1000 then table.remove(histograms[key],1)end end function
metrics.get_memory_usage():number local success,memory_kb=pcall(function()return
collectgarbage('count')end)if success then return memory_kb else return 0 end
end function metrics.log_metric(entry:MetricEntry__DARKLUA_TYPE_E)table.insert(
metric_history,entry)if#metric_history>1000 then table.remove(metric_history,1)
end local log_line=string.format('[METRIC] %s | %s | %s | %.2fms | %s | %s',
entry.timestamp,entry.platform,entry.operation,entry.duration_ms,entry.success
and'SUCCESS'or'FAILED',entry.memory_used_kb and(entry.memory_used_kb..'KB')or
'N/A')if not entry.success and entry.error then log_line=log_line..' | error: '
..tostring(entry.error)end if not metrics.silent_mode then if io and io.stdout
then io.stdout:write(log_line..'\n')io.stdout:flush()else print(log_line)end end
end function metrics.get_performance_summary(platform:string):
JsonObject__DARKLUA_TYPE_C local platform_metrics:{MetricEntry__DARKLUA_TYPE_E}=
{}local total_duration=0 local success_count=0 local error_count=0 for _,entry
in ipairs(metric_history)do if entry.platform==platform then table.insert(
platform_metrics,entry)total_duration=total_duration+entry.duration_ms if entry.
success then success_count=success_count+1 else error_count=error_count+1 end
end end local total_count=#platform_metrics return{platform=platform,
total_executions=total_count,success_count=success_count,error_count=error_count
,success_rate=total_count>0 and(success_count/total_count)or 0,
average_duration_ms=total_count>0 and(total_duration/total_count)or 0,
total_duration_ms=total_duration}end function metrics.with_monitoring<T>(fn:(any
)->T,operation:string,platform:string,input:any):T local timer=metrics.
start_timer(operation,platform)local success,result=pcall(fn,input)if success
then metrics.end_timer(timer,true)return result else metrics.end_timer(timer,
false,tostring(result))error(result)end end return metrics end function
__BUNDLE_MODULES.g()local types=__BUNDLE_MODULES.load('a')local subset:
SubsetModule__DARKLUA_TYPE_K={}::any function subset.is_array(t:any):boolean if
type(t)~='table'then return false end if#t>0 then return true end local k=next(t
)return k~=nil and type(k)=='number'end function subset.apply_subsetting(data:
JsonValue__DARKLUA_TYPE_H,fields:JsonValue__DARKLUA_TYPE_H):
JsonValue__DARKLUA_TYPE_H?if type(data)=='table'and type(fields)=='table'then
local fields_obj=fields::JsonObject__DARKLUA_TYPE_I if subset.is_array(data)then
if fields_obj['*']~=nil then local data_arr=data::JsonArray__DARKLUA_TYPE_J
local filtered_items:JsonArray__DARKLUA_TYPE_J={}local item_spec=fields_obj['*']
for _,item in ipairs(data_arr)do local subset_item=subset.apply_subsetting(item,
item_spec)if subset_item~=nil and type(subset_item)=='table'and next(subset_item
::JsonObject__DARKLUA_TYPE_I)~=nil then table.insert(filtered_items,subset_item)
end end return filtered_items end else local data_obj=data::
JsonObject__DARKLUA_TYPE_I local result:JsonObject__DARKLUA_TYPE_I={}for
field_name,field_spec in pairs(fields_obj)do local data_value=data_obj[
field_name]if data_value~=nil then if field_spec==true then result[field_name]=
data_value elseif type(field_spec)=='table'then local subset_value=subset.
apply_subsetting(data_value,field_spec)if subset_value~=nil then result[
field_name]=subset_value end end end end return result end end return nil end
return subset end function __BUNDLE_MODULES.h()local types=__BUNDLE_MODULES.
load('a')local helpers=__BUNDLE_MODULES.load('d')local error_handler=
__BUNDLE_MODULES.load('e')local metrics=__BUNDLE_MODULES.load('f')local subset=
__BUNDLE_MODULES.load('g')local mapper_template:
MapperTemplateModule__DARKLUA_TYPE_S={}function mapper_template.
_process_with_config(config:MapperConfig__DARKLUA_TYPE_Q,raw:
PayloadObject__DARKLUA_TYPE_O):ProcessResult__DARKLUA_TYPE_R local timeout=
config.timeout or 30 local process_result=error_handler.with_time_budget(
function()return config.process(raw,config.helpers)end,timeout,config.platform..
'_main_processing')if not process_result.success then return{success=false,data=
nil,error=process_result.error or'Processing failed'}end return{success=true,
data=process_result.result,error=nil}end function mapper_template.create_mapper(
config:MapperConfig__DARKLUA_TYPE_Q):(params:MapperParams__DARKLUA_TYPE_P)->
JsonObject__DARKLUA_TYPE_M assert(config.platform,
'MapperConfig.platform is required')assert(config.constants,
'MapperConfig.constants is required')assert(config.constants.PLATFORM,
'MapperConfig.constants.PLATFORM is required')assert(config.process,
'MapperConfig.process is required')assert(type(config.process)=='function',
'MapperConfig.process must be a function')return function(params:
MapperParams__DARKLUA_TYPE_P):JsonObject__DARKLUA_TYPE_M local raw=params.
payload local field_subset=params.field_subset if field_subset~=nil then if
type(field_subset)~='table'then local subset_error=string.format(
[[[%s] Invalid field_subset: expected JSON object or nil, got %s]],config.
platform,type(field_subset))error(subset_error)end local function
validate_subset_structure(obj:any,path:string):(boolean,string?)if type(obj)~=
'table'then return false,'Invalid subset structure at '..path end for key,value
in pairs(obj::{[any]:any})do if type(key)~='string'then return false,
'Invalid key type at '..path..': expected string, got '..type(key)end local
value_type=type(value)if value_type=='boolean'then elseif value_type=='table'
then local is_valid,err=validate_subset_structure(value,path..'.'..key)if not
is_valid then return false,err end else return false,string.format(
[[Invalid value type at %s.%s: expected boolean or object, got %s]],path,key,
value_type)end end return true,nil end local is_valid,validation_error=
validate_subset_structure(field_subset,'field_subset')if not is_valid then
error(string.format('[%s] %s',config.platform,validation_error))end end local
timer=metrics.start_timer('map_'..config.platform..'_data',config.platform)if
config.metrics_config and config.metrics_config.detailed then metrics.
record_counter('mapper_invocations',config.platform,1)end local payload_data:
PayloadObject__DARKLUA_TYPE_O=raw if type(payload_data)~='table'then error(
[[Invalid payload type: expected object with string values, got ]]..type(
payload_data))end local validation_target=nil local parse_error=nil if
payload_data then local first_endpoint=next(payload_data)if first_endpoint and
payload_data[first_endpoint]then local success,parsed_or_error=pcall(helpers.
parse_json_string,payload_data[first_endpoint])if success then if
parsed_or_error then validation_target=parsed_or_error else parse_error=string.
format([[Failed to parse JSON from endpoint '%s': invalid JSON format]],
first_endpoint)end else parse_error=string.format(
"Failed to parse JSON from endpoint '%s': %s",first_endpoint,tostring(
parsed_or_error))end else parse_error=
'Payload object is empty or contains no valid endpoints'end else parse_error=
'Invalid payload type: expected string or object, got '..type(payload_data)end
if parse_error then metrics.end_timer(timer,false,parse_error)if config.
metrics_config and config.metrics_config.detailed then metrics.record_counter(
'mapper_parse_failures',config.platform,1)end error(parse_error)end local
validation if validation_target and type(validation_target)=='table'then
validation=error_handler.validate_input(validation_target,config.platform)else
local type_error=string.format(
'Invalid data structure: expected JSON object, got %s',type(validation_target))
validation={is_valid=false,errors={type_error}}end if not validation.is_valid
then local error_msg=string.format('[%s] Data validation failed: %s',config.
platform,table.concat(validation.errors,'; '))metrics.end_timer(timer,false,
error_msg)if config.metrics_config and config.metrics_config.detailed then
metrics.record_counter('mapper_validation_failures',config.platform,1)end error(
error_msg)end local process_result=mapper_template._process_with_config(config,
payload_data)if not process_result.success then metrics.end_timer(timer,false,
process_result.error)if config.metrics_config and config.metrics_config.detailed
then metrics.record_counter('mapper_failures',config.platform,1)end error(
process_result.error or'Unknown processing error')end local data=process_result.
data local final_data=data if field_subset then local subset_data=subset.
apply_subsetting(data,field_subset)if subset_data then final_data=subset_data if
config.metrics_config and config.metrics_config.detailed then metrics.
record_counter('field_subsetting_applied',config.platform,1)end end end metrics.
end_timer(timer,true)if config.metrics_config and config.metrics_config.detailed
then metrics.record_counter('mapper_successes',config.platform,1)end local
prepared_result=helpers.prepare_for_json(final_data)return prepared_result end
end return mapper_template end function __BUNDLE_MODULES.i()return{PLATFORM=
'github'}end function __BUNDLE_MODULES.j()local types=__BUNDLE_MODULES.load('a')
local date:DateModule__DARKLUA_TYPE_T={}::any function date.format_iso_date(year
:number,month:number,day:number):string local y_str=tostring(year)local m_str=
month<10 and('0'..month)or tostring(month)local d_str=day<10 and('0'..day)or
tostring(day)return table.concat({y_str,'-',m_str,'-',d_str,'T00:00:00Z'})end
function date.validate_iso_z(s:string?):string?if not s or s==''then return nil
end local year,month,day,hour,min,sec=string.match(s,
'^(%d%d%d%d)%-(%d%d)%-(%d%d)T(%d%d):(%d%d):(%d%d)Z$')if year and month and day
and hour and min and sec then local y,m,d=tonumber(year),tonumber(month),
tonumber(day)local h,mn,sc=tonumber(hour),tonumber(min),tonumber(sec)if m>=1 and
m<=12 and d>=1 and d<=31 and h>=0 and h<=23 and mn>=0 and mn<=59 and sc>=0 and
sc<=59 then return s end end return nil end function date.normalize_iso_to_z(s:
string?):string?if not s or s==''then return nil end local year,month,day,hour,
min,sec=string.match(s,'^(%d%d%d%d)%-(%d%d)%-(%d%d)T(%d%d):(%d%d):(%d%d)')if
year and month and day and hour and min and sec then return string.format(
'%04d-%02d-%02dT%02d:%02d:%02dZ',tonumber(year),tonumber(month),tonumber(day),
tonumber(hour),tonumber(min),tonumber(sec))end return nil end function date.
parse_us_date_inline(s:string?):string?if not s or s==''then return s end local
m,d,y=s:match('^(%d+)/(%d+)/(%d+)$')if m then local month,day,year=tonumber(m),
tonumber(d),tonumber(y)if month>=1 and month<=12 and day>=1 and day<=31 then
return date.format_iso_date(year,month,day)end end return s end function date.
unix_to_iso(timestamp:number?):string?if not timestamp then return nil end local
YEAR_2000_UNIX=946684800 local YEAR_2100_UNIX=4102444800 local
MILLISECOND_THRESHOLD=10000000000 if timestamp>=MILLISECOND_THRESHOLD then
timestamp=math.floor(timestamp/1000)end if timestamp<YEAR_2000_UNIX or timestamp
>YEAR_2100_UNIX then return nil end return os.date('!%Y-%m-%dT%H:%M:%SZ',
timestamp)end return date end function __BUNDLE_MODULES.k()local types=
__BUNDLE_MODULES.load('a')local common=__BUNDLE_MODULES.load('d')local date=
__BUNDLE_MODULES.load('j')local constants=__BUNDLE_MODULES.load('i')local
helpers:HelpersTable__DARKLUA_TYPE_X={}function helpers.normalize_example(value:
string?):string?if value==nil then return nil end local trimmed=string.gsub(
value,'^%s*(.-)%s*$','%1')if trimmed==''then return nil end return string.lower(
trimmed)end return helpers end end local types=__BUNDLE_MODULES.load('a')local
common=__BUNDLE_MODULES.load('d')local error_handler=__BUNDLE_MODULES.load('e')
local metrics=__BUNDLE_MODULES.load('f')local mapper_template=__BUNDLE_MODULES.
load('h')local constants=__BUNDLE_MODULES.load('i')local helpers=
__BUNDLE_MODULES.load('k')type JsonValue=JsonValue__DARKLUA_TYPE_b type
JsonObject=JsonObject__DARKLUA_TYPE_c type JsonArray=JsonArray__DARKLUA_TYPE_d
type PayloadObject=PayloadObject__DARKLUA_TYPE_e local function
process_github_data(raw:PayloadObject,platform_helpers:any):JsonObject local
github_string=raw.default if not github_string then return{}end local
github_data=common.parse_json_string(github_string)if not github_data then
return{}end local body=github_data.body or github_data or{}local data:JsonObject
={}common.put(data,'username',common.safe_string(body.login))common.put(data,
'display_name',common.safe_string(body.name))common.put(data,'bio',common.
safe_string(body.bio))common.put(data,'company',common.safe_string(body.company)
)common.put(data,'location',common.safe_string(body.location))common.put(data,
'email',common.safe_string(body.email))common.put(data,'twitter_username',common
.safe_string(body.twitter_username))common.put(data,'blog',common.safe_string(
body.blog))common.put(data,'public_repos',common.safe_number(body.public_repos))
common.put(data,'public_gists',common.safe_number(body.public_gists))common.put(
data,'followers',common.safe_number(body.followers))common.put(data,'following',
common.safe_number(body.following))common.put(data,'created_at',common.
safe_string(body.created_at))common.put(data,'updated_at',common.safe_string(
body.updated_at))common.put(data,'hireable',body.hireable)common.put(data,
'two_factor_authentication',body.two_factor_authentication==true)common.put(data
,'disk_usage_kb',common.safe_number(body.disk_usage))if body.plan and type(body.
plan)=='table'then common.put(data,'plan_name',common.safe_string(body.plan.name
))common.put(data,'private_repos_limit',common.safe_number(body.plan.
private_repos))end return data end return mapper_template.create_mapper({
platform='github',constants=constants,process=process_github_data,helpers=
helpers,timeout=30,metrics_config={detailed=false}})