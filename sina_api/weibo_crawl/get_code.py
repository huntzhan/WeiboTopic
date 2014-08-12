
import urllib


def get_url(client_id, redirect_uri):
    AUTHORIZE_URL = "https://api.weibo.com/oauth2/authorize"
    arguments = {
        'client_id': client_id,
        'redirect_uri': redirect_uri,
    }
    encoded_arguments = urllib.urlencode(arguments)
    print AUTHORIZE_URL + "?" + encoded_arguments


if __name__ == "__main__":
    get_url(
        '158481545',
        'http://haoxun.org',
    )
