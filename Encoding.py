import gzip

from Messages import Response


def compress_response_body(encoding: str, response: Response):
    encoding_no_comma = encoding[:-1] if encoding[-1] == ',' else encoding
    if encoding_no_comma == 'gzip':
        response.body = gzip.compress(response.body.encode())
        response.headers['Content-Length'] = len(response.body)
        response.headers['Content-Encoding'] = 'gzip'
        response.compressed_body = True
