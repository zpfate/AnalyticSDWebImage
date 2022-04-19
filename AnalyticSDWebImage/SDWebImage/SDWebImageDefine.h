/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"

typedef void(^SDWebImageNoParamsBlock)(void);
typedef NSString * SDWebImageContextOption NS_EXTENSIBLE_STRING_ENUM;
typedef NSDictionary<SDWebImageContextOption, id> SDWebImageContext;
typedef NSMutableDictionary<SDWebImageContextOption, id> SDWebImageMutableContext;

#pragma mark - Image scale

/**
 Return the image scale factor for the specify key, supports file name and url key.
 This is the built-in way to check the scale factor when we have no context about it. Because scale factor is not stored in image data (It's typically from filename).
 However, you can also provide custom scale factor as well, see `SDWebImageContextImageScaleFactor`.

 @param key The image cache key
 @return The scale factor for image
 */
FOUNDATION_EXPORT CGFloat SDImageScaleFactorForKey(NSString * _Nullable key);

/**
 Scale the image with the scale factor for the specify key. If no need to scale, return the original image.
 This works for `UIImage`(UIKit) or `NSImage`(AppKit). And this function also preserve the associated value in `UIImage+Metadata.h`.
 @note This is actually a convenience function, which firstlly call `SDImageScaleFactorForKey` and then call `SDScaledImageForScaleFactor`, kept for backward compatibility.

 @param key The image cache key
 @param image The image
 @return The scaled image
 */
FOUNDATION_EXPORT UIImage * _Nullable SDScaledImageForKey(NSString * _Nullable key, UIImage * _Nullable image);

/**
 Scale the image with the scale factor. If no need to scale, return the original image.
 This works for `UIImage`(UIKit) or `NSImage`(AppKit). And this function also preserve the associated value in `UIImage+Metadata.h`.
 
 @param scale The image scale factor
 @param image The image
 @return The scaled image
 */
FOUNDATION_EXPORT UIImage * _Nullable SDScaledImageForScaleFactor(CGFloat scale, UIImage * _Nullable image);

#pragma mark - WebCache Options

/// WebCache options
typedef NS_OPTIONS(NSUInteger, SDWebImageOptions) {
    /**
     * By default, when a URL fail to be downloaded, the URL is blacklisted so the library won't keep trying.
     * This flag disable this blacklisting.
     * 如果一个url加载图片失败会加入黑名单 不在重试, 设置该选项取消黑名单限制
     */
    SDWebImageRetryFailed = 1 << 0,
    
    /**
     * By default, image downloads are started during UI interactions, this flags disable this feature,
     * leading to delayed download on UIScrollView deceleration for instance.
     * 默认情况是在UI交互的时候下载图片
     * 低优先级, 设置该选项在UIScrollView滑动减速时才开始下载图片
     */
    SDWebImageLowPriority = 1 << 1,
    
    /**
     * This flag enables progressive download, the image is displayed progressively during download as a browser would do.
     * By default, the image is only displayed once completely downloaded.
     * 设置该选项 图片会逐渐展示,像浏览器一样
     * 默认是图片下载完成展示
     */
    SDWebImageProgressiveLoad = 1 << 2,
    
    /**
     * Even if the image is cached, respect the HTTP response cache control, and refresh the image from remote location if needed.
     * The disk caching will be handled by NSURLCache instead of SDWebImage leading to slight performance degradation.
     * This option helps deal with images changing behind the same request URL, e.g. Facebook graph api profile pics.
     * If a cached image is refreshed, the completion block is called once with the cached image and again with the final image.
     *
     * Use this flag only if you can't make your URLs static with embedded cache busting parameter.
     * 一个图片缓存了,还是会重新请求.并且缓存侧略依据NSURLCache而不是SDWebImage.URL不变,图片会更新时使用
     * 需要服务端支持cache control
     */
    SDWebImageRefreshCached = 1 << 3,
    
    /**
     * In iOS 4+, continue the download of the image if the app goes to background. This is achieved by asking the system for
     * extra time in background to let the request finish. If the background task expires the operation will be cancelled.
     * 启动后台下载
     */
    SDWebImageContinueInBackground = 1 << 4,
    
    /**
     * Handles cookies stored in NSHTTPCookieStore by setting
     * NSMutableURLRequest.HTTPShouldHandleCookies = YES;
     * 可以控制存在NSHTTPCookieStore的cookies
     */
    SDWebImageHandleCookies = 1 << 5,
    
    /**
     * Enable to allow untrusted SSL certificates.
     * Useful for testing purposes. Use with caution in production.
     * 允许不安全的SSL证书,在正式环境中慎用
     */
    SDWebImageAllowInvalidSSLCertificates = 1 << 6,
    
    /**
     * By default, images are loaded in the order in which they were queued. This flag moves them to
     * the front of the queue.
     * 高优先级下载 会把该任务放置到队列前面
     */
    SDWebImageHighPriority = 1 << 7,
    
    /**
     * By default, placeholder images are loaded while the image is loading. This flag will delay the loading
     * of the placeholder image until after the image has finished loading.
     * 默认情况下,占位图会在图片下载的时候显示.这个flag开启会延迟占位图显示的时间,等到图片下载完成之后才会显示占位图
     */
    SDWebImageDelayPlaceholder = 1 << 8,
    
    /**
     * We usually don't apply transform on animated images as most transformers could not manage animated images.
     * Use this flag to transform them anyway.
     * 我们通常不会对变换动图，因为大多数变缓都无法管理动图。 ??
     */
    SDWebImageTransformAnimatedImage = 1 << 9,
    
    /**
     * By default, image is added to the imageView after download. But in some cases, we want to
     * have the hand before setting the image (apply a filter or add it with cross-fade animation for instance)
     * Use this flag if you want to manually set the image in the completion when success
     * (默认情况下，图像在下载后添加到imageView。
     * 但在某些情况下，我们希望在设置图像之前可以手动修改动画
     */
    SDWebImageAvoidAutoSetImage = 1 << 10,
    
    /**
     * By default, images are decoded respecting their original size. On iOS, this flag will scale down the
     * images to a size compatible with the constrained memory of devices.
     * This flag take no effect if `SDWebImageAvoidDecodeImage` is set. And it will be ignored if `SDWebImageProgressiveLoad` is set.
     * 默认情况下，图像将根据其原始大小进行解码。此标志将图像缩小到与设备的受限内存兼容的大小;
     * 主要用于加载大图片时避免高内存导致闪退
     */
    SDWebImageScaleDownLargeImages = 1 << 11,
    
    /**
     * By default, we do not query image data when the image is already cached in memory. This mask can force to query image data at the same time. However, this query is asynchronously unless you specify `SDWebImageQueryMemoryDataSync`
     * 默认情况下，当图像已经缓存在内存中时，我们不查询图像数据。
     * 此掩码可以强制同时查询图像数据。除非指定“SDWebImageQueryMemoryDataSync”，否则此查询是异步的`
     */
    SDWebImageQueryMemoryData = 1 << 12,
    
    /**
     * By default, when you only specify `SDWebImageQueryMemoryData`, we query the memory image data asynchronously. Combined this mask as well to query the memory image data synchronously.
     * @note Query data synchronously is not recommend, unless you want to ensure the image is loaded in the same runloop to avoid flashing during cell reusing.
     *  (默认情况下，当您仅指定“SDWebImageQueryMemoryData”时，我们将异步查询内存映像数据。结合该掩模，实现了对内存图像数据的同步查询。)
     */
    SDWebImageQueryMemoryDataSync = 1 << 13,
    
    /**
     * By default, when the memory cache miss, we query the disk cache asynchronously. This mask can force to query disk cache (when memory cache miss) synchronously.
     * @note These 3 query options can be combined together. For the full list about these masks combination, see wiki page.
     * @note Query data synchronously is not recommend, unless you want to ensure the image is loaded in the same runloop to avoid flashing during cell reusing.
     * (默认情况下，当内存缓存丢失时，我们异步查询磁盘缓存。此掩码可以强制同步查询磁盘缓存（当内存缓存丢失时）。
     */
    SDWebImageQueryDiskDataSync = 1 << 14,
    
    /**
     * By default, when the cache missed, the image is load from the loader. This flag can prevent this to load from cache only.
     * (默认情况下，当缓存丢失时，从加载程序加载图像。
     * 此标志可设置为仅从缓存加载此文件
     */
    SDWebImageFromCacheOnly = 1 << 15,
    
    /**
     * By default, we query the cache before the image is load from the loader. This flag can prevent this to load from loader only.
     * 默认情况下，我们在从加载程序加载图像之前查询缓存。此标志可防止仅从加载程序加载此文件。
     */
    SDWebImageFromLoaderOnly = 1 << 16,
    
    /**
     * By default, when you use `SDWebImageTransition` to do some view transition after the image load finished, this transition is only applied for image download from the network. This mask can force to apply view transition for memory and disk cache as well.
     * 默认情况下，当使用“SDWebImageTransition”在映像加载完成后执行某些视图转换时，此转换仅在管理器的回调是异步的（从网络或磁盘缓存查询）时应用于映像.此掩码可以强制对任何情况应用视图转换，如内存缓存查询或同步磁盘缓存查询
 
     */
    SDWebImageForceTransition = 1 << 17,
    
    /**
     * By default, we will decode the image in the background during cache query and download from the network. This can help to improve performance because when rendering image on the screen, it need to be firstly decoded. But this happen on the main queue by Core Animation.
     * However, this process may increase the memory usage as well. If you are experiencing a issue due to excessive memory consumption, This flag can prevent decode the image.
     * 默认情况下，我们将在缓存查询和从网络下载期间在后台解码图像。这有助于提高性能，因为在屏幕上渲染图像时，需要首先对其进行解码。但是，这个过程也会增加内存的使用。如果由于内存消耗过多而遇到问题，此标志可能会阻止对图像进行解码。
     */
    SDWebImageAvoidDecodeImage = 1 << 18,
    
    /**
     * By default, we decode the animated image. This flag can force decode the first frame only and produece the static image.
     * 默认情况下，我们解码动画图像。此标志只能强制解码第一帧并生成静态图像
     */
    SDWebImageDecodeFirstFrameOnly = 1 << 19,
    
    /**
     * By default, for `SDAnimatedImage`, we decode the animated image frame during rendering to reduce memory usage. However, you can specify to preload all frames into memory to reduce CPU usage when the animated image is shared by lots of imageViews.
     * This will actually trigger `preloadAllAnimatedImageFrames` in the background queue(Disk Cache & Download only).
     * 默认情况下，对于“SDAnimatedImage”，我们在渲染期间解码动画图像帧以减少内存使用。但是，您可以指定将所有帧预加载到内存中，以在大量ImageView共享动画图像时减少CPU使用量。这实际上会触发后台队列中的“prelodallanimatedImageFrames”（仅限于磁盘缓存和下载）
     */
    SDWebImagePreloadAllFrames = 1 << 20
};


#pragma mark - Context Options

/**
 A String to be used as the operation key for view category to store the image load operation. This is used for view instance which supports different image loading process. If nil, will use the class name as operation key. (NSString *)
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextSetImageOperationKey;

/**
 A SDWebImageManager instance to control the image download and cache process using in UIImageView+WebCache category and likes. If not provided, use the shared manager (SDWebImageManager *)
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextCustomManager;

/**
 A id<SDImageTransformer> instance which conforms `SDImageTransformer` protocol. It's used for image transform after the image load finished and store the transformed image to cache. If you provide one, it will ignore the `transformer` in manager and use provided one instead. (id<SDImageTransformer>)
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextImageTransformer;

/**
 A CGFloat raw value which specify the image scale factor. The number should be greater than or equal to 1.0. If not provide or the number is invalid, we will use the cache key to specify the scale factor. (NSNumber)
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextImageScaleFactor;

/**
 A SDImageCacheType raw value which specify the cache type when the image has just been downloaded and will be stored to the cache. Specify `SDImageCacheTypeNone` to disable cache storage; `SDImageCacheTypeDisk` to store in disk cache only; `SDImageCacheTypeMemory` to store in memory only. And `SDImageCacheTypeAll` to store in both memory cache and disk cache.
 If not provide or the value is invalid, we will use `SDImageCacheTypeAll`. (NSNumber)
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextStoreCacheType;

/**
 A Class object which the instance is a `UIImage/NSImage` subclass and adopt `SDAnimatedImage` protocol. We will call `initWithData:scale:options:` to create the instance (or `initWithAnimatedCoder:scale:` when using progressive download) . If the instance create failed, fallback to normal `UIImage/NSImage`.
 This can be used to improve animated images rendering performance (especially memory usage on big animated images) with `SDAnimatedImageView` (Class).
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextAnimatedImageClass;

/**
 A id<SDWebImageDownloaderRequestModifier> instance to modify the image download request. It's used for downloader to modify the original request from URL and options. If you provide one, it will ignore the `requestModifier` in downloader and use provided one instead. (id<SDWebImageDownloaderRequestModifier>)
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextDownloadRequestModifier;

/**
 A id<SDWebImageCacheKeyFilter> instance to convert an URL into a cache key. It's used when manager need cache key to use image cache. If you provide one, it will ignore the `cacheKeyFilter` in manager and use provided one instead. (id<SDWebImageCacheKeyFilter>)
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextCacheKeyFilter;

/**
 A id<SDWebImageCacheSerializer> instance to convert the decoded image, the source downloaded data, to the actual data. It's used for manager to store image to the disk cache. If you provide one, it will ignore the `cacheSerializer` in manager and use provided one instead. (id<SDWebImageCacheSerializer>)
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextCacheSerializer;
